#include <math.h>
#include <stdio.h>

#include "ctr.h"
#include "drill_test.h"
#include "solution.c"

#define NEAR(a, b) (fabs((a) - (b)) < 1e-9)

int main(void)
{
	ctr_t *c;
	unsigned long long d = 99;

	/* [1] 만들기 */
	CHECK(ctr_create(2) == NULL);
	c = ctr_create(1000);
	CHECK(c != NULL);
	CHECK(ctr_feed(NULL, 1, &d) == CTR_FIRST);
	CHECK(ctr_feed(c, 1, NULL) == CTR_FIRST);
	ctr_destroy(c);
	ctr_destroy(NULL);

	/* [2] 정상 증가: 1000 → 1063 → 1063 */
	c = ctr_create(~0ULL);
	CHECK(ctr_feed(c, 1000, &d) == CTR_FIRST && d == 0);
	CHECK(ctr_feed(c, 1063, &d) == CTR_OK && d == 63);
	CHECK(ctr_feed(c, 1063, &d) == CTR_OK && d == 0);
	ctr_destroy(c);

	/* [3] 32비트 카운터가 최대값을 넘어 되돌아감 → WRAP */
	c = ctr_create(4294967295ULL);
	ctr_feed(c, 4294967290ULL, &d);
	CHECK(ctr_feed(c, 5, &d) == CTR_WRAP);
	CHECK(d == 11); /* 290→295 가 5, 295→0 이 1, 0→5 가 5 */
	CHECK(ctr_feed(c, 20, &d) == CTR_OK && d == 15); /* 되돌아간 뒤에는 5 가 새 기준 */
	ctr_destroy(c);

	/* [4] 재시작 → RESET: 이전 값이 작으면 되돌아간 게 아니라 0 부터 다시 센 것이다 */
	c = ctr_create(4294967295ULL);
	ctr_feed(c, 1000, &d);
	CHECK(ctr_feed(c, 7, &d) == CTR_RESET);
	CHECK(d == 7);
	CHECK(ctr_feed(c, 10, &d) == CTR_OK && d == 3);
	ctr_destroy(c);

	/* [5] WRAP/RESET 경계: max=100 이면 윗 1/4 은 이전 값 > 75 */
	c = ctr_create(100);
	ctr_feed(c, 76, &d);
	CHECK(ctr_feed(c, 10, &d) == CTR_WRAP && d == (100 - 76) + 1 + 10);
	ctr_destroy(c);
	c = ctr_create(100);
	ctr_feed(c, 75, &d);
	CHECK(ctr_feed(c, 10, &d) == CTR_RESET && d == 10);
	ctr_destroy(c);

	/* [6] 가중 평균: 평균의 평균(25)이 아니라 건수를 가중한 37 */
	{
		const double avg[] = {10.0, 40.0};
		const unsigned long cnt[] = {100, 900};
		CHECK(NEAR(wavg(avg, cnt, 2), 37.0));
		CHECK(NEAR(wavg(avg, cnt, 1), 10.0));
	}
	{
		const double avg[] = {10.0, 40.0};
		const unsigned long zero[] = {0, 0};
		const unsigned long one[] = {0, 5};
		CHECK(NEAR(wavg(avg, zero, 2), 0.0));
		CHECK(NEAR(wavg(avg, one, 2), 40.0)); /* 건수 0 인 구간은 영향이 없다 */
		CHECK(NEAR(wavg(avg, one, 0), 0.0));
		CHECK(NEAR(wavg(NULL, one, 2), 0.0));
	}

	/* [7] 합계 검산 */
	{
		long diff = 77;
		CHECK(sum_check(63, 48, 15, 0, 0, &diff) == 1 && diff == 0);
		CHECK(sum_check(63, 40, 15, 0, 0, &diff) == 0 && diff == 8);  /* 8건이 어디로 갔나 (진행 중이거나 누락) */
		CHECK(sum_check(60, 48, 15, 0, 0, &diff) == 0 && diff == -3); /* 중복 카운트 */
		CHECK(sum_check(63, 40, 15, 0, 8, NULL) == 1);                /* 진행 중 8건을 알면 맞는다 */
	}

	return drill_report();
}
