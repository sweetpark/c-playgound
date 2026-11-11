#include <signal.h>
#include <stdint.h>
#include <unistd.h>

#include "drill_test.h"
#include "solution.c"

static void on_alarm(int s)
{
	static const char m[] = "FAIL (timeout: infinite loop?)\n";
	(void)s;
	if (write(1, m, sizeof(m) - 1) < 0) { /* ignore */ }
	_exit(1);
}

int main(void)
{
	signal(SIGALRM, on_alarm);
	alarm(10);

	CHECK(count_steps(0, 10, 3) == 4);                 /* 0 3 6 9 */
	CHECK(count_steps(0, 9, 3) == 4);                  /* 끝값 포함 */
	CHECK(count_steps(5, 5, 1) == 1);
	CHECK(count_steps(5, 4, 1) == 0);                  /* start > end */
	CHECK(count_steps(0, 10, 0) == 0);                 /* step 0 은 무한 루프의 원인 */
	CHECK(count_steps(0, 0, 7) == 1);
	CHECK(count_steps(1, 100, 1) == 100);

	/* size_t 최대값 근처: for (i = s; i <= end; i += step) 은 i 가 랩어라운드되어 영원히 돈다 */
	CHECK(count_steps(SIZE_MAX - 5, SIZE_MAX, 2) == 3);       /* MAX-5, MAX-3, MAX-1 */
	CHECK(count_steps(SIZE_MAX - 1, SIZE_MAX, 1) == 2);
	CHECK(count_steps(0, SIZE_MAX, SIZE_MAX) == 2);           /* 0, MAX */
	CHECK(count_steps(1, SIZE_MAX, SIZE_MAX) == 1);
	CHECK(count_steps(0, SIZE_MAX, 2) == SIZE_MAX / 2 + 1);
	CHECK(count_steps(1, SIZE_MAX, 1) == SIZE_MAX);           /* 딱 표현 가능한 최대 */
	CHECK(count_steps(0, SIZE_MAX, 1) == SIZE_MAX);           /* SIZE_MAX + 1 은 표현 불가: SIZE_MAX 로 포화 */

	return drill_report();
}
