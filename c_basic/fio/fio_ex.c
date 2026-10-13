#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/*
 * 사용법: ./fio_ex_v2 <src> <dst>
 *  - src의 공백, 탭, 개행을 제거해서 dst에 저장
 *  - dst와 같은 디렉토리에 임시 파일을 만들어 쓰고, 성공하면 rename으로 교체
 *    (도중에 실패해도 기존 dst는 손상되지 않고, 임시 파일은 삭제됨)
 */
int main(int argc, char *argv[]){

	FILE *src_fp = NULL;
	FILE *tmp_fp = NULL;
	int src_fd = -1;
	int tmp_fd = -1;

	const char *src_file_name = NULL;
	const char *dst_file_name = NULL;
	char tmp_name[PATH_MAX];
	int tmp_created = 0;		/* 임시 파일이 디스크에 남아있는지 */

	struct stat src_st;
	struct stat dst_st;

	size_t i = 0;
	int c;
	int n;
	int ret = -1;			/* 마지막에 성공할 때만 0으로 바꿈 */
	char buffer[1024];


	/* 1. 인자로 src, dst 받기 */
	if(argc != 3){
		fprintf(stderr, "usage: %s <src> <dst>\n", argv[0]);
		goto CLEAN;
	}
	src_file_name = argv[1];
	dst_file_name = argv[2];


	/* 2. src 파일 상태 체크 (존재해야 하고 일반 파일이어야 함) */
	if(stat(src_file_name, &src_st) != 0){
		perror("stat src");
		goto CLEAN;
	}
	if(S_ISDIR(src_st.st_mode)){
		fprintf(stderr, "src is a directory: %s\n", src_file_name);
		goto CLEAN;
	}
	if(!S_ISREG(src_st.st_mode)){
		fprintf(stderr, "src is not a regular file: %s\n", src_file_name);
		goto CLEAN;
	}

	/* 2. dst 파일 상태 체크 (없으면 새로 생성, 있으면 일반 파일만 교체 허용) */
	if(stat(dst_file_name, &dst_st) == 0){
		if(S_ISDIR(dst_st.st_mode)){
			fprintf(stderr, "dst is a directory: %s\n", dst_file_name);
			goto CLEAN;
		}
		if(!S_ISREG(dst_st.st_mode)){
			fprintf(stderr, "dst is not a regular file: %s\n", dst_file_name);
			goto CLEAN;
		}
		if(src_st.st_dev == dst_st.st_dev && src_st.st_ino == dst_st.st_ino){
			fprintf(stderr, "src and dst are the same file\n");
			goto CLEAN;
		}
	}
	else if(errno != ENOENT){	/* 없는 것 말고 다른 이유로 실패 (권한 등) */
		perror("stat dst");
		goto CLEAN;
	}


	/* src 열기 (읽기 전용) */
	src_fd = open(src_file_name, O_RDONLY);
	if(src_fd < 0){
		perror("open src");
		goto CLEAN;
	}

	/* 3. dst와 같은 디렉토리에 임시 파일 생성 (rename이 같은 파일시스템에서 이뤄지도록) */
	n = snprintf(tmp_name, sizeof(tmp_name), "%s.tmpXXXXXX", dst_file_name);
	if(n < 0 || (size_t)n >= sizeof(tmp_name)){
		fprintf(stderr, "dst name too long\n");
		goto CLEAN;
	}

	tmp_fd = mkstemp(tmp_name);	/* 0600, O_EXCL로 생성, tmp_name이 실제 이름으로 바뀜 */
	if(tmp_fd < 0){
		perror("mkstemp");
		goto CLEAN;
	}
	tmp_created = 1;

	/* src와 같은 권한으로 맞추기 */
	if(fchmod(tmp_fd, src_st.st_mode & 0777) != 0){
		perror("fchmod tmp");
		goto CLEAN;
	}

	/* fdopen 성공 이후에는 fclose가 fd까지 닫아주므로 fp로 관리 */
	src_fp = fdopen(src_fd, "r");
	if(src_fp == NULL){
		perror("fdopen src");
		goto CLEAN;
	}
	src_fd = -1;

	tmp_fp = fdopen(tmp_fd, "w");
	if(tmp_fp == NULL){
		perror("fdopen tmp");
		goto CLEAN;
	}
	tmp_fd = -1;


	/* 공백, 탭, 개행을 제외하고 buffer에 모으기 */
	while((c = fgetc(src_fp)) != EOF){
		if( c == ' ' || c == '\t' || c == '\n') continue;

		/* buffer가 꽉 차면 파일에 쓰고 비운 뒤 이어서 읽기 */
		if( i >= sizeof(buffer) ){
			if( fwrite(buffer, 1, i, tmp_fp) != i ){
				fprintf(stderr, "fwrite fail\n");
				goto CLEAN;
			}
			i = 0;
		}
		buffer[i] = c;
		i++;
	}

	/* EOF가 아니라 읽기 에러로 끝난 경우 */
	if( ferror(src_fp) ){
		fprintf(stderr, "read error: %s\n", src_file_name);
		goto CLEAN;
	}

	/* 남아있는 길이(i)만큼 파일에 쓰기 */
	if( i > 0 && fwrite(buffer, 1, i, tmp_fp) != i ){
		fprintf(stderr, "fwrite fail\n");
		goto CLEAN;
	}

	/* 6. 버퍼를 비우고 디스크에 반영 (여기서 쓰기 오류가 드러날 수 있음) */
	if( fflush(tmp_fp) != 0 ){
		perror("fflush tmp");
		goto CLEAN;
	}
	if( fsync(fileno(tmp_fp)) != 0 ){
		perror("fsync tmp");
		goto CLEAN;
	}

	/* 6. fclose 반환값 검사 (실패해도 fp는 이미 닫힌 것이므로 NULL로 돌려놓고 에러 처리) */
	n = fclose(tmp_fp);
	tmp_fp = NULL;
	if( n != 0 ){
		perror("fclose tmp");
		goto CLEAN;
	}

	/* 3. 임시 파일을 dst로 교체 (같은 파일시스템이므로 원자적) */
	if( rename(tmp_name, dst_file_name) != 0 ){
		perror("rename");
		goto CLEAN;
	}
	tmp_created = 0;

	ret = 0;

CLEAN:
	/* 열려있는 것만 정리. fp가 있으면 fclose, 없으면 fd를 직접 close */
	if( src_fp != NULL ){
		if( fclose(src_fp) != 0 ){
			perror("fclose src");
			ret = -1;
		}
	}
	else if( src_fd >= 0 ){
		close(src_fd);
	}

	if( tmp_fp != NULL ){
		if( fclose(tmp_fp) != 0 ){
			perror("fclose tmp");
		}
		ret = -1;
	}
	else if( tmp_fd >= 0 ){
		close(tmp_fd);
	}

	/* 실패했으면 남아있는 임시 파일 삭제 */
	if( tmp_created ){
		unlink(tmp_name);
	}

	return ret;
}
