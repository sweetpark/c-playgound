/* P107 alarmfsm — 요구사항은 problem.md. alarmfsm.h 의 선언을 보고 채운다. */
#include <stdlib.h>

#include "alarmfsm.h"

struct alarm_fsm {
	/* TODO: 임계 4개, 켜짐 여부, 연속 초과/정상 횟수, 발생/해제 횟수 */
	int unused;
};

alarm_fsm_t *alarm_create(int set_thr, int clear_thr, int set_n, int clear_n)
{
	(void)set_thr;
	(void)clear_thr;
	(void)set_n;
	(void)clear_n;
	/* TODO */
	return NULL;
}

alm_event_t alarm_feed(alarm_fsm_t *a, int value)
{
	(void)a;
	(void)value;
	/* TODO */
	return ALM_NONE;
}

int alarm_is_on(const alarm_fsm_t *a)
{
	(void)a;
	/* TODO */
	return 0;
}

void alarm_counts(const alarm_fsm_t *a, int *occurred, int *cleared)
{
	(void)a;
	(void)occurred;
	(void)cleared;
	/* TODO */
}

void alarm_destroy(alarm_fsm_t *a)
{
	(void)a;
	/* TODO */
}
