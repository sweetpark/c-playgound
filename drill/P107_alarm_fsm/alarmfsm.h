/* P107 alarmfsm — 임계값으로 알람을 켜고 끄는 상태 머신. 구현은 solution.c */
#ifndef ALARMFSM_H
#define ALARMFSM_H

typedef enum {
	ALM_NONE = 0,     /* 상태가 바뀌지 않았다 */
	ALM_OCCURRED = 1, /* 꺼짐 → 켜짐 */
	ALM_CLEARED = 2   /* 켜짐 → 꺼짐 */
} alm_event_t;

typedef struct alarm_fsm alarm_fsm_t;

/* set_thr   : 값이 이 이상이면 "초과"로 센다
 * clear_thr : 값이 이 미만이면 "정상"으로 센다 (clear_thr <= set_thr 이어야 한다)
 * set_n     : 연속 몇 번 초과해야 켜는가 (>= 1)
 * clear_n   : 연속 몇 번 정상이어야 끄는가 (>= 1)
 * 인자가 잘못되면 NULL */
alarm_fsm_t *alarm_create(int set_thr, int clear_thr, int set_n, int clear_n);

/* 측정값 하나를 먹이고, 이번에 상태가 바뀌었는지 돌려준다. a 가 NULL 이면 ALM_NONE */
alm_event_t alarm_feed(alarm_fsm_t *a, int value);

/* 지금 켜져 있으면 1 */
int alarm_is_on(const alarm_fsm_t *a);

/* 지금까지 낸 발생/해제 횟수 (NULL 인자는 무시) */
void alarm_counts(const alarm_fsm_t *a, int *occurred, int *cleared);

void alarm_destroy(alarm_fsm_t *a);

#endif
