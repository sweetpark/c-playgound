#include <stdio.h>

/* P034 sum_file — 모범답안 */
int sum_file(FILE *fp, long *out)
{
	long sum = 0, v;
	int r;

	if (fp == NULL || out == NULL)          /* 진입부에서 인자 검사 */
		return -1;

	while ((r = fscanf(fp, "%ld", &v)) == 1) {
		if (__builtin_add_overflow(sum, v, &sum))   /* signed 오버플로는 UB 이므로 연산 전/중에 검사 */
			return -4;
	}
	/* 루프가 끝난 이유를 반환값으로: 0 = 형식 불일치(숫자가 아닌 토큰), EOF = 입력 끝 또는 오류 */
	if (r == 0)
		return -3;
	if (ferror(fp) || !feof(fp))            /* EOF 인데 입력 끝 표시가 없다 = 읽기 오류 (쓰기 전용 스트림 등) */
		return -2;

	*out = sum;
	return 0;
}
