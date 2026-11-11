#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

#define SENT 123456

/* 입력 s 에 대해 반환값과 (성공 시) 출력값을 검사. 실패 시 out 은 SENT 그대로여야 한다 */
static void run(const char *s, int want_ret, int want_val)
{
	int out = SENT;
	int r = my_atoi(s, &out);
	CHECK(r == want_ret);
	if (want_ret == 0) CHECK(out == want_val);
	else CHECK(out == SENT);
}

int main(void)
{
	int out = SENT;

	run("123", 0, 123);
	run("0", 0, 0);
	run("-45", 0, -45);
	run("+7", 0, 7);
	run("007", 0, 7);
	run("2147483647", 0, INT_MAX);
	run("-2147483648", 0, INT_MIN);

	/* int 범위 밖 (long 은 64비트라 strtol 은 성공한다 — 직접 범위 검사 필요) */
	run("2147483648", -2, 0);
	run("-2147483649", -2, 0);
	/* long 범위 밖 (ERANGE) */
	run("99999999999999999999", -2, 0);
	run("-99999999999999999999", -2, 0);

	/* 형식 오류 */
	run("", -1, 0);
	run("abc", -1, 0);
	run("12a", -1, 0);   /* 뒤쪽 쓰레기 */
	run("1 2", -1, 0);
	run(" 1", -1, 0);    /* 앞 공백 거절 */
	run("1 ", -1, 0);    /* 뒤 공백 거절 */
	run("-", -1, 0);
	run("+", -1, 0);
	run("0x10", -1, 0);  /* 10진수만 */

	CHECK(my_atoi(NULL, &out) == -1);
	CHECK(out == SENT);
	CHECK(my_atoi("1", NULL) == -1);

	/* 함정: 이전에 남은 errno 가 결과를 오염시키면 안 된다 (호출 전 errno = 0 으로) */
	errno = ERANGE;
	out = SENT;
	CHECK(my_atoi("5", &out) == 0);
	CHECK(out == 5);

	return drill_report();
}
