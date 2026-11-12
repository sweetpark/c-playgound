/* P108 ctr — 누적 카운터의 차분, 평균 합산, 합계 검산. 구현은 solution.c */
#ifndef CTR_H
#define CTR_H

typedef enum {
	CTR_FIRST = 0, /* 첫 측정: 이전 값이 없어 증가량을 모른다 (delta = 0) */
	CTR_OK = 1,    /* 정상 증가 (같은 값이면 delta = 0) */
	CTR_WRAP = 2,  /* 최대값을 넘어 0 부근으로 되돌아갔다 */
	CTR_RESET = 3  /* 프로세스 재시작 등으로 0 부터 다시 셌다 */
} ctr_kind_t;

typedef struct ctr ctr_t;

/* 카운터가 가질 수 있는 최대값(max). 값의 범위는 0..max, max 다음은 0 으로 돌아간다. max < 3 이면 NULL */
ctr_t *ctr_create(unsigned long long max);

/* 새로 읽은 누적값 value(0..max)를 먹이고, 이번 구간의 증가량을 *delta 에 넣는다.
 *  - 첫 측정             → CTR_FIRST, delta 0
 *  - value >= 이전 값    → CTR_OK,    delta = value - 이전
 *  - value <  이전 값    → "이전 값이 범위의 윗 1/4 안에 있었으면 WRAP, 아니면 RESET"
 *        윗 1/4 : 이전 값 > max - max/4  (정수 나눗셈)
 *        WRAP : delta = (max - 이전) + 1 + value
 *        RESET: delta = value  (재시작 후 센 만큼만 인정)
 *  어느 경우든 "이전 값"은 value 로 갱신된다. c 또는 delta 가 NULL 이면 CTR_FIRST 를 돌려주고 아무것도 안 한다 */
ctr_kind_t ctr_feed(ctr_t *c, unsigned long long value, unsigned long long *delta);

void ctr_destroy(ctr_t *c);

/* 구간별 평균(avg[i])과 건수(cnt[i]) n 개를 하나로 합친 평균. 건수 합이 0 이거나 n <= 0 이면 0.0 */
double wavg(const double *avg, const unsigned long *cnt, int n);

/* 불변식 검사: req == rsp + timeout + cancel + inflight 인가.
 * *diff 에 req - (rsp + timeout + cancel + inflight) 를 부호 있는 값으로 넣는다 (diff 가 NULL 이면 생략).
 * 맞으면 1, 틀리면 0 */
int sum_check(unsigned long req, unsigned long rsp, unsigned long timeout, unsigned long cancel,
              unsigned long inflight, long *diff);

#endif
