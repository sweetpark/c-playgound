#include <stdio.h>

#include "alarmfsm.h"
#include "drill_test.h"
#include "solution.c"

/* 값 배열을 먹이고, 이벤트를 'O'(발생) 'C'(해제) '.'(없음) 문자열로 모은다 */
static void run(alarm_fsm_t *a, const int *v, int n, char *out)
{
	int i;

	for (i = 0; i < n; i++) {
		alm_event_t e = alarm_feed(a, v[i]);
		out[i] = (e == ALM_OCCURRED) ? 'O' : (e == ALM_CLEARED) ? 'C' : '.';
	}
	out[n] = '\0';
}

static int streq(const char *a, const char *b)
{
	while (*a && *a == *b) {
		a++;
		b++;
	}
	return *a == *b;
}

int main(void)
{
	char ev[64];
	int o, c;
	alarm_fsm_t *a;

	/* [1] 잘못된 인자 */
	CHECK(alarm_create(80, 90, 1, 1) == NULL); /* clear > set */
	CHECK(alarm_create(90, 80, 0, 1) == NULL);
	CHECK(alarm_create(90, 80, 1, 0) == NULL);
	CHECK(alarm_feed(NULL, 100) == ALM_NONE);
	CHECK(alarm_is_on(NULL) == 0);
	alarm_destroy(NULL);

	/* [2] 임계 하나(set==clear): 경계에서 깜박인다 — 7번 측정에 6건 */
	{
		static const int v[] = {88, 91, 89, 92, 89, 91, 88};
		a = alarm_create(91, 91, 1, 1);
		CHECK(a != NULL);
		run(a, v, 7, ev);
		CHECK(streq(ev, ".OCOCOC"));
		alarm_counts(a, &o, &c);
		CHECK(o == 3 && c == 3);
		alarm_destroy(a);
	}

	/* [3] 히스테리시스(set 91, clear 80): 같은 입력에 2건 */
	{
		static const int v[] = {88, 91, 89, 92, 89, 91, 88, 79};
		a = alarm_create(91, 80, 1, 1);
		run(a, v, 8, ev);
		CHECK(streq(ev, ".O.....C"));
		CHECK(alarm_is_on(a) == 0);
		alarm_counts(a, &o, &c);
		CHECK(o == 1 && c == 1);
		alarm_destroy(a);
	}

	/* [4] 켜진 동안 완충 지대(80~90)의 값은 상태를 바꾸지 않는다 */
	{
		static const int v[] = {95, 85, 85, 85, 85};
		a = alarm_create(91, 80, 1, 1);
		run(a, v, 5, ev);
		CHECK(streq(ev, "O...."));
		CHECK(alarm_is_on(a) == 1);
		alarm_destroy(a);
	}

	/* [5] 연속 N회: 초과 3번이 연속이어야 켠다. 한 번 끊기면 처음부터 */
	{
		static const int v[] = {95, 70, 95, 96, 97};
		a = alarm_create(91, 80, 3, 1);
		run(a, v, 5, ev);
		CHECK(streq(ev, "....O"));
		alarm_destroy(a);
	}
	{
		static const int v[] = {95, 96, 70, 95, 96};
		a = alarm_create(91, 80, 3, 1);
		run(a, v, 5, ev);
		CHECK(streq(ev, "....."));
		CHECK(alarm_is_on(a) == 0);
		alarm_destroy(a);
	}

	/* [6] 꺼짐 쪽 연속 M회: 정상이 2번 연속이어야 끈다. 완충 지대 값이 끼면 센 것이 리셋된다 */
	{
		static const int v[] = {95, 70, 85, 70, 70};
		a = alarm_create(91, 80, 1, 2);
		run(a, v, 5, ev);
		CHECK(streq(ev, "O...C")); /* 70 다음 85 가 끼면 정상 횟수가 0 으로 돌아가서, 마지막 70,70 에서야 끈다 */
		alarm_destroy(a);
	}

	/* [7] 켜진 뒤 초과가 계속돼도 발생을 다시 내지 않는다 */
	{
		static const int v[] = {95, 96, 97, 98, 99, 100};
		a = alarm_create(91, 80, 1, 1);
		run(a, v, 6, ev);
		CHECK(streq(ev, "O....."));
		alarm_counts(a, &o, &c);
		CHECK(o == 1 && c == 0);
		alarm_destroy(a);
	}

	/* [8] 해제 직후 다시 처음부터: 켜는 데 다시 N회가 필요하다 */
	{
		static const int v[] = {95, 95, 70, 95, 95, 95};
		a = alarm_create(91, 80, 2, 1);
		run(a, v, 6, ev);
		CHECK(streq(ev, ".OC.O."));
		alarm_counts(a, &o, &c);
		CHECK(o == 2 && c == 1);
		alarm_destroy(a);
	}

	/* [9] 두 번째 켜짐에서도 "정상 M번 연속"을 다시 요구한다 (지난 해제 때 센 횟수가 남아 있으면 안 된다) */
	{
		static const int v[] = {95, 70, 70, 95, 70, 70};
		a = alarm_create(91, 80, 1, 2);
		run(a, v, 6, ev);
		CHECK(streq(ev, "O.CO.C"));
		alarm_counts(a, &o, &c);
		CHECK(o == 2 && c == 2);
		alarm_destroy(a);
	}

	return drill_report();
}
