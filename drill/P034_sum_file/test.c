#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

static FILE *mk(const char *s)
{
	FILE *fp = tmpfile();
	if (!fp) return NULL;
	fputs(s, fp);
	rewind(fp);
	return fp;
}

static void run(const char *content, int want_ret, long want_sum)
{
	FILE *fp = mk(content);
	long out = 777;
	int r;
	CHECK(fp != NULL);
	if (!fp) return;
	r = sum_file(fp, &out);
	CHECK(r == want_ret);
	if (want_ret == 0) CHECK(out == want_sum);
	else CHECK(out == 777);                       /* 실패하면 out 불변 */
	fclose(fp);
}

int main(void)
{
	long out = 777;
	FILE *w;

	run("1 2 3\n4", 0, 10);
	run("", 0, 0);                                /* 빈 파일은 오류가 아니라 합 0 */
	run("  \n \n", 0, 0);
	run("-5 5 100\n-100\n", 0, 0);
	run("42", 0, 42);
	run("1 x 3", -3, 0);                          /* 숫자가 아닌 토큰 */
	run("1 2 3abc", -3, 0);
	run("1 2\n-", -3, 0);
	run("9223372036854775807 1", -4, 0);          /* 합 오버플로 */
	run("-9223372036854775807 -2", -4, 0);

	/* 읽기 오류와 EOF 의 구분: 쓰기 전용으로 연 스트림을 읽으면 오류 상태 */
	w = fopen("/dev/null", "w");
	CHECK(w != NULL);
	if (w) {
		CHECK(sum_file(w, &out) == -2);
		CHECK(out == 777);
		fclose(w);
	}

	/* 인자 오류 */
	{
		FILE *fp = mk("1 2");
		CHECK(sum_file(NULL, &out) == -1);
		CHECK(sum_file(fp, NULL) == -1);
		if (fp) fclose(fp);
	}
	return drill_report();
}
