#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

typedef enum {
	SUM_OK = 0,
	SUM_EPARAM,
	SUM_EOPEN,
	SUM_EREAD,
	SUM_EINVAL,
	SUM_ERANGE
} sum_err_t;

/*
 * 1. 파일의 각 줄에는 10진수 정수가 1개씩 저장
 * 2. 함수의 리턴값은 에러 코드로만 사용, 합계는 sum 인자로, 에러가 발생한 줄번호는 err_line으로 전달
 * 3. 문자열의 숫자변환은 strtol() 사용
 * 4. 합계를 더하는 과정에서 long 범위를 넘는경우도 SUM_ERANGE 체크
 * 파일읽기가 끝났을때 파일의 끝인지 읽기에러인지는 ferror() 로 구분
 * 에러 리턴시 sum의 값은 변경하지 않고, 어떤 경우에도 열린파일은 닫고 리턴
 * 함수 내부에서 메시지 출력, exit(), abort() 호출금지
 *
 * [err_line]
 *   - 인자 오류(SUM_EPARAM) : 변경하지 않음
 *   - 그 외 : 에러가 없으면 0, SUM_EOPEN 이면 0 (파일 안의 줄이 아님),
 *             줄 단위 에러(SUM_EINVAL / SUM_ERANGE / SUM_EREAD)는 파일의 1부터 시작하는 줄번호
 */
sum_err_t sum_file(const char* _path, long *_sum, size_t *_err_line){

	FILE *fp = NULL;
	char buf[1024];
	char *end = NULL;
	size_t line_no = 0;
	size_t len = 0;
	long num = 0;
	long total = 0;
	sum_err_t ret = SUM_OK;

	if (_path == NULL || _sum == NULL || _err_line == NULL){
		return SUM_EPARAM;
	}
	*_err_line = 0;

	fp = fopen(_path, "r");
	if (fp == NULL){
		return SUM_EOPEN;
	}

	while(fgets(buf, sizeof(buf), fp) != NULL){
		line_no++;

		/* 개행 없이 버퍼가 가득 찬 경우 = 한 줄이 버퍼보다 길다 (마지막 줄이 개행 없이 끝나는 경우는 EOF 라서 허용) */
		len = strlen(buf);
		if (len > 0 && buf[len - 1] != '\n' && !feof(fp)){
			ret = SUM_EINVAL;
			goto CLEANUP;
		}

		errno = 0;
		num = strtol(buf, &end, 10);

		if (end == buf){                        /* 숫자가 하나도 없음 ("abc", 빈 줄) */
			ret = SUM_EINVAL;
			goto CLEANUP;
		}
		if (*end == '\r'){                      /* CRLF 파일 */
			end++;
		}
		if (*end != '\n' && *end != '\0'){      /* 숫자 뒤에 문자가 남음 ("12abc") */
			ret = SUM_EINVAL;
			goto CLEANUP;
		}
		if (errno == ERANGE){                   /* 한 줄의 값이 long 범위를 벗어남 */
			ret = SUM_ERANGE;
			goto CLEANUP;
		}

		/* 더하기 전에 넘치는지 검사 (더한 뒤에는 이미 미정의 동작) */
		if ((num > 0 && total > LONG_MAX - num) || (num < 0 && total < LONG_MIN - num)){
			ret = SUM_ERANGE;
			goto CLEANUP;
		}
		total += num;
	}

	if (ferror(fp)){                            /* EOF 가 아니라 읽기 에러로 끝남 */
		line_no++;                              /* 읽다가 실패한 줄 */
		ret = SUM_EREAD;
		goto CLEANUP;
	}

	*_sum = total;                              /* 성공했을 때만 sum 을 갱신 (에러는 위에서 CLEANUP 으로 빠짐) */

CLEANUP:
	fclose(fp);
	if (ret != SUM_OK){
		*_err_line = line_no;
	}
	return ret;
}

const char *sum_strerror(sum_err_t err){
	switch(err){
		case SUM_OK:
			return "SUM_OK";
		case SUM_EPARAM:
			return "SUM_EPARAM";
		case SUM_EOPEN:
			return "SUM_EOPEN";
		case SUM_EREAD:
			return "SUM_EREAD";
		case SUM_EINVAL:
			return "SUM_EINVAL";
		case SUM_ERANGE:
			return "SUM_ERANGE";
		default:
			return "UNKNOWN";
	}
}


#define SUM_SENTINEL 123456789L     /* 에러일 때 sum 이 바뀌지 않는지 확인하기 위한 초기값 */

/* 사용법: ./sum 파일 [파일 ...]   (각 파일의 합계를 순서대로 출력) */
int main(int argc, char **argv){

	long sum = 0;
	size_t err_line = 0;
	sum_err_t ret = SUM_OK;
	int i = 0;
	int fail_cnt = 0;

	if(argc < 2){
		fprintf(stderr, "usage: %s file [file ...]\n", argv[0]);
		return 2;
	}

	for(i = 1; i < argc; i++){
		sum = SUM_SENTINEL;
		err_line = 0;

		ret = sum_file(argv[i], &sum, &err_line);
		if(ret == SUM_OK){
			printf("%s: sum = %ld\n", argv[i], sum);
		}else{
			fail_cnt++;
			printf("%s: error = %s(%d), line = %zu\n", argv[i], sum_strerror(ret), (int)ret, err_line);
			if(sum != SUM_SENTINEL){            /* 요구사항: 에러 리턴 시 sum 은 변경 금지 */
				printf("  !! sum was modified on error: %ld\n", sum);
			}
		}
	}

	/* 인자 오류 확인: NULL 인자는 SUM_EPARAM 이어야 한다 (구현이 틀리면 여기서 크래시) */
	fflush(stdout);
	ret = sum_file(NULL, &sum, &err_line);
	printf("param check (path NULL): %s\n", sum_strerror(ret));
	fflush(stdout);
	ret = sum_file(argv[1], NULL, &err_line);
	printf("param check (sum NULL) : %s\n", sum_strerror(ret));
	fflush(stdout);
	ret = sum_file(argv[1], &sum, NULL);
	printf("param check (line NULL): %s\n", sum_strerror(ret));

	return fail_cnt != 0;
}
