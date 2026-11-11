#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "drill_test.h"
#include "solution.c"

static void on_alarm(int s)
{
	static const char m[] = "FAIL (timeout/deadlock)\n";
	(void)s;
	if (write(1, m, sizeof(m) - 1) < 0) { /* ignore */ }
	_exit(1);
}

static int check_n(int n)
{
	long out[64];
	int i, ok = 1;
	memset(out, 0xEE, sizeof(out));
	if (run_squares(n, out) != 0) return 0;
	for (i = 0; i < n; i++)
		if (out[i] != (long)i * i) ok = 0;
	return ok;
}

int main(void)
{
	long out[4] = {-1, -1, -1, -1};
	int round;

	signal(SIGALRM, on_alarm);
	alarm(60);

	/* 스레드 i 는 자기 i 를 받아야 한다 (루프 변수의 주소를 넘기면 스레드가 도는 사이 값이 바뀐다) */
	CHECK(check_n(1));
	CHECK(check_n(8));
	CHECK(check_n(64));
	for (round = 0; round < 200; round++)
		if (!check_n(16)) { CHECK(0); break; }
	CHECK(round == 200);

	/* 거절: out 은 건드리지 않는다 */
	CHECK(run_squares(0, out) == -1);
	CHECK(run_squares(-3, out) == -1);
	CHECK(run_squares(65, out) == -1);
	CHECK(run_squares(4, NULL) == -1);
	CHECK(out[0] == -1 && out[3] == -1);

	return drill_report();
}
