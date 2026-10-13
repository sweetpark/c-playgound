#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

/* 1. 인자로 파일명 + 출력파일명 받아서 동작 */
int main(int argc, char **argv){
	char *src_name = NULL;
	char *dst_name = NULL;

	int src_fd = -1;
	int dst_fd = -1;

	FILE *src_fp = NULL;
	FILE *dst_fp = NULL;

	int total_lines = 0;	/* 전체 줄 수 */
	int max_len = 0;	/* 가장 긴 줄의 길이 (개행 제외) */
	int cur_len = 0;	/* 현재 줄의 길이 */
	int current_line = 1;	/* 줄 번호는 1부터 */
	int width = 0;		/* 줄 번호 자릿수 */
	int at_line_start = 1;

	int out;		/* fgetc는 int를 반환 (EOF와 구분) */
	int ret = -1;

	int ret_val = 0;

	if(argc != 3){
		fprintf(stderr, "usage: %s <src> <dst>\n", argv[0]);
		return -1;
	}
	src_name = argv[1];
	dst_name = argv[2];

	/* src는 읽기 전용 (이미 존재해야 함) */
	src_fd = open(src_name, O_RDONLY);
	if(src_fd < 0){
		perror(src_name);
		goto CLEAN;
	}

	/* dst는 새로 생성 (이미 있으면 실패) */
	dst_fd = open(dst_name, O_WRONLY | O_CREAT | O_EXCL, 0644);
	if(dst_fd < 0){
		perror(dst_name);
		goto CLEAN;
	}

	/* fdopen 성공 이후에는 fclose가 fd까지 닫아주므로 fd를 -1로 돌려놓음 */
	src_fp = fdopen(src_fd, "r");
	if(src_fp == NULL){
		perror("fdopen src");
		goto CLEAN;
	}
	src_fd = -1;

	dst_fp = fdopen(dst_fd, "w");
	if(dst_fp == NULL){
		perror("fdopen dst");
		goto CLEAN;
	}
	dst_fd = -1;


	/* 2. 첫번째 읽기) 전체 줄수와 가장 긴줄의 길이 계산 */
	while((out = fgetc(src_fp)) != EOF){
		if(out != '\n') {
			cur_len++;
		}else{
			if(max_len < cur_len){
				max_len = cur_len;
			}
			cur_len = 0;
			total_lines++;
		}
	}
	if(ferror(src_fp)){
		fprintf(stderr, "read error: %s\n", src_name);
		goto CLEAN;
	}

	/* 마지막 줄에 개행이 없는 경우도 한 줄로 계산 */
	if(cur_len > 0){
		if(max_len < cur_len){
			max_len = cur_len;
		}
		total_lines++;
	}

	/* 줄 번호 자릿수 (전체 줄 수에 맞춤, 최소 1) */
	width = 1;
	for(out = total_lines; out >= 10; out /= 10){
		width++;
	}


	/* 3. 두번째읽기) 줄번호: 원본 내용 으로 dst파일에 저장 */
	ret_val = fseek(src_fp, 0L, SEEK_SET);
	if (ret_val != 0){		/* fseek는 실패 시 -1 반환, 원인은 errno(EBADF, EINVAL 등) */
		perror("fseek");
		goto CLEAN;
	}

	fprintf(dst_fp, "전체 줄 수 : %d, 가장 긴 줄 수 : %d\n\n", total_lines, max_len);

	while((out = fgetc(src_fp)) != EOF){
		/* 줄의 첫 글자일 때만 줄 번호를 찍음 (마지막 개행 뒤에 빈 번호가 생기지 않음) */
		if(at_line_start){
			fprintf(dst_fp, "%*d : ", width, current_line);
			current_line++;
			at_line_start = 0;
		}

		fputc(out, dst_fp);

		if(out == '\n'){
			at_line_start = 1;
		}
	}
	if(ferror(src_fp)){
		fprintf(stderr, "read error: %s\n", src_name);
		goto CLEAN;
	}

	/* 버퍼를 비우면서 쓰기 오류가 있었는지 확인 */
	if(fflush(dst_fp) != 0 || ferror(dst_fp)){
		fprintf(stderr, "write error: %s\n", dst_name);
		goto CLEAN;
	}

	ret = 0;

CLEAN:
	if(src_fp != NULL){
		if(fclose(src_fp) != 0){
			perror("fclose src");
			ret = -1;
		}
	}
	else if(src_fd >= 0){
		close(src_fd);
	}

	if(dst_fp != NULL){
		if(fclose(dst_fp) != 0){
			perror("fclose dst");
			ret = -1;
		}
	}
	else if(dst_fd >= 0){
		close(dst_fd);
	}

	return ret;
}
