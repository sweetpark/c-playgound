#include <stdio.h>

#include "drill_test.h"
#include "shed.h"
#include "solution.c"

int main(void)
{
	shed_t *s;
	long ad = 0, rj = 0;
	int i, n;

	/* [1] 만들기 */
	CHECK(shed_create(0) == NULL);
	CHECK(shed_create(-3) == NULL);
	CHECK(shed_admit(NULL, 0) == 0);
	shed_destroy(NULL);

	/* [2] 한 구간(1초)에 한도까지만: limit 5 */
	s = shed_create(5);
	CHECK(s != NULL);
	CHECK(shed_admit(s, 0) == 1);
	CHECK(shed_admit(s, 100) == 1);
	CHECK(shed_admit(s, 200) == 1);
	CHECK(shed_admit(s, 300) == 1);
	CHECK(shed_admit(s, 400) == 1);
	CHECK(shed_admit(s, 500) == 0);
	CHECK(shed_admit(s, 999) == 0);
	CHECK(shed_admit(s, 1000) == 1); /* 다음 구간: 센 개수가 0 으로 */
	shed_stats(s, &ad, &rj);
	CHECK(ad == 6 && rj == 2);
	shed_stats(s, NULL, NULL);
	shed_destroy(s);

	/* [3] 고정 구간의 한계: 구간 경계에 몰리면 0.2초 안에 한도의 2배가 통과한다 */
	s = shed_create(5);
	n = 0;
	for (i = 0; i < 5; i++)
		n += shed_admit(s, 900 + i * 20);   /* 900~980ms: 0번 구간 */
	for (i = 0; i < 5; i++)
		n += shed_admit(s, 1000 + i * 20);  /* 1000~1080ms: 1번 구간 */
	CHECK(n == 10);
	shed_destroy(s);

	/* [4] 몇 구간을 건너뛰어도 새 구간으로: 첫 요청은 항상 받는다 (시작 시각이 큰 값이어도) */
	s = shed_create(2);
	CHECK(shed_admit(s, 50000) == 1);
	CHECK(shed_admit(s, 50100) == 1);
	CHECK(shed_admit(s, 50200) == 0);
	CHECK(shed_admit(s, 90000) == 1);
	shed_destroy(s);

	/* [5] 시계가 뒤로 가면 새 구간으로 취급하지 않고 지금 구간으로 센다 */
	s = shed_create(2);
	CHECK(shed_admit(s, 2500) == 1);
	CHECK(shed_admit(s, 2600) == 1);
	CHECK(shed_admit(s, 100) == 0);  /* 이전 구간 시각이지만 지금 구간(2)의 한도 소진 */
	shed_destroy(s);

	/* [6] 사용률 → 거절 비율 */
	CHECK(shed_cut_percent(0) == 0);
	CHECK(shed_cut_percent(49) == 0);
	CHECK(shed_cut_percent(50) == 20);
	CHECK(shed_cut_percent(69) == 20);
	CHECK(shed_cut_percent(70) == 50);
	CHECK(shed_cut_percent(89) == 50);
	CHECK(shed_cut_percent(90) == 90);
	CHECK(shed_cut_percent(100) == 90);
	CHECK(shed_cut_percent(-5) == 0);
	CHECK(shed_cut_percent(150) == 90);

	/* [7] 거절 비율대로 정확히 거른다 (100개 중) */
	{
		static const int cuts[] = {0, 20, 50, 90, 100};
		static const int want[] = {0, 20, 50, 90, 100};
		int k;
		for (k = 0; k < 5; k++) {
			n = 0;
			for (i = 0; i < 100; i++)
				n += shed_pick_reject(cuts[k], (unsigned)i);
			CHECK(n == want[k]);
		}
		CHECK(shed_pick_reject(-1, 7) == 0);
		CHECK(shed_pick_reject(150, 7) == 1);
		CHECK(shed_pick_reject(20, 100) == 1); /* seq % 100 == 0 */
		CHECK(shed_pick_reject(20, 120) == 0); /* seq % 100 == 20 */
	}

	/* [8] 백오프: 지수 증가, 상한, 오버플로 없음 */
	CHECK(backoff_ms(0, 100, 5000, 0, 0) == 100);
	CHECK(backoff_ms(1, 100, 5000, 0, 0) == 200);
	CHECK(backoff_ms(2, 100, 5000, 0, 0) == 400);
	CHECK(backoff_ms(5, 100, 5000, 0, 0) == 3200);
	CHECK(backoff_ms(6, 100, 5000, 0, 0) == 5000);
	CHECK(backoff_ms(40, 100, 5000, 0, 0) == 5000);
	CHECK(backoff_ms(100000, 100, 5000, 0, 0) == 5000);
	CHECK(backoff_ms(-1, 100, 5000, 0, 0) == 100);
	CHECK(backoff_ms(3, 0, 5000, 0, 0) == 0);
	CHECK(backoff_ms(0, 100, 60, 0, 0) == 60); /* 첫 값도 상한을 넘지 않는다 */

	/* [9] 지터: [lo, d] 안의 값, rnd 로 결정 */
	CHECK(backoff_ms(0, 100, 5000, 50, 0) == 50);
	CHECK(backoff_ms(0, 100, 5000, 50, 1000) == 100);
	CHECK(backoff_ms(0, 100, 5000, 50, 500) == 75);
	CHECK(backoff_ms(0, 100, 5000, 50, 1001) == 50);  /* rnd % 1001 == 0 */
	CHECK(backoff_ms(0, 100, 5000, 150, 500) == 50);  /* 지터 150 → 100 으로 자름: lo = 0 */
	CHECK(backoff_ms(0, 100, 5000, -10, 500) == 100); /* 지터 -10 → 0 */
	{
		long v;
		for (i = 0; i < 2000; i++) {
			v = backoff_ms(i % 12, 100, 5000, 30, (unsigned)(i * 7919));
			if (v < 0 || v > 5000) {
				CHECK(0);
				break;
			}
		}
	}

	return drill_report();
}
