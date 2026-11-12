/* P107 alarmfsm 모범답안 */
#include <stdlib.h>

#include "alarmfsm.h"

struct alarm_fsm {
	int set_thr, clear_thr, set_n, clear_n;
	int on;
	int above;  /* 꺼진 상태에서 연속 초과 횟수 */
	int below;  /* 켜진 상태에서 연속 정상 횟수 */
	int n_occ, n_clr;
};

alarm_fsm_t *alarm_create(int set_thr, int clear_thr, int set_n, int clear_n)
{
	alarm_fsm_t *a;

	if (clear_thr > set_thr || set_n < 1 || clear_n < 1)
		return NULL;
	a = calloc(1, sizeof(*a));
	if (a == NULL)
		return NULL;
	a->set_thr = set_thr;
	a->clear_thr = clear_thr;
	a->set_n = set_n;
	a->clear_n = clear_n;
	return a;
}

alm_event_t alarm_feed(alarm_fsm_t *a, int value)
{
	if (a == NULL)
		return ALM_NONE;
	if (!a->on) {
		a->above = (value >= a->set_thr) ? a->above + 1 : 0;
		if (a->above >= a->set_n) {
			a->on = 1;
			a->above = 0;
			a->below = 0;
			a->n_occ++;
			return ALM_OCCURRED;
		}
	} else {
		a->below = (value < a->clear_thr) ? a->below + 1 : 0;
		if (a->below >= a->clear_n) {
			a->on = 0;
			a->below = 0;
			a->above = 0;
			a->n_clr++;
			return ALM_CLEARED;
		}
	}
	return ALM_NONE;
}

int alarm_is_on(const alarm_fsm_t *a)
{
	return a != NULL && a->on;
}

void alarm_counts(const alarm_fsm_t *a, int *occurred, int *cleared)
{
	if (a == NULL)
		return;
	if (occurred)
		*occurred = a->n_occ;
	if (cleared)
		*cleared = a->n_clr;
}

void alarm_destroy(alarm_fsm_t *a)
{
	free(a);
}
