/* P109 shed — 과부하 제어 부품 세 개: 초당 한도(고정 구간), 사용률 단계별 거절 비율, 재시도 백오프. 구현은 solution.c */
#ifndef SHED_H
#define SHED_H

typedef struct shed shed_t;

/* 초당 tps_limit 건까지 받는 고정 구간(fixed window) 제한기. tps_limit < 1 이면 NULL */
shed_t *shed_create(int tps_limit);

/* now_ms(밀리초 시각, 0 이상)에 요청 하나가 왔다. 받으면 1, 거절하면 0.
 *  - 구간 = now_ms / 1000 (1초). 구간이 바뀌면 센 개수를 0 으로 되돌린다.
 *  - 한 구간에 tps_limit 건까지 받고, 그 뒤는 거절한다.
 *  - 시계가 뒤로 가서 지금 구간보다 이전 구간의 시각이 오면, 새 구간으로 보지 않고 지금 구간으로 취급한다. */
int shed_admit(shed_t *s, long now_ms);

/* 지금까지 받은 / 거절한 요청 수 (NULL 인자는 무시) */
void shed_stats(const shed_t *s, long *admitted, long *rejected);

void shed_destroy(shed_t *s);

/* CPU 사용률(%) → 거절 비율(%).  50 미만 0, 50~69 20, 70~89 50, 90 이상 90.
 * 0 미만은 0, 100 초과는 90 으로 본다 */
int shed_cut_percent(int cpu_pct);

/* 거절 비율 cut_percent(%)가 주어졌을 때 순번 seq 의 요청을 거절하나 (결정적).
 * (seq % 100) < cut_percent 이면 1(거절). cut_percent <= 0 은 항상 0, >= 100 은 항상 1 */
int shed_pick_reject(int cut_percent, unsigned seq);

/* 재시도 대기 시간(ms). attempt 번째 재시도(0 부터):
 *  d = base_ms * 2^attempt 를 cap_ms 로 자른다 (attempt 가 아무리 커도 오버플로 없이 cap_ms).
 *  지터: 대기 = lo + (d - lo) * (rnd % 1001) / 1000,  lo = d * (100 - jitter_pct) / 100  (jitter_pct 는 0..100 으로 자른다)
 *  base_ms < 1 이면 0.  attempt < 0 은 0 으로 본다.  d 는 cap_ms 를 넘지 않는다 */
long backoff_ms(int attempt, long base_ms, long cap_ms, int jitter_pct, unsigned rnd);

#endif
