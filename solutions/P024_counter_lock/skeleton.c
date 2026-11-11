#include <pthread.h>
#include <stddef.h>

/* P024 counter_lock — 요구사항은 problem.md */
typedef struct {
	pthread_mutex_t mu;
	long v;
} counter_t;

/* ERRORCHECK 뮤텍스로 초기화, v = 0. 0 성공 / -1 실패 */
int counter_init(counter_t *c)
{
	(void)c;
	/* TODO */
	return -1;
}

/* v += d  (모든 스레드에서 안전하게) */
void counter_add(counter_t *c, long d)
{
	(void)c;
	(void)d;
	/* TODO */
}

/* v 읽기 (읽는 쪽도 락) */
long counter_get(counter_t *c)
{
	(void)c;
	/* TODO */
	return -1;
}

/* 뮤텍스 파괴. pthread_mutex_destroy 의 결과를 그대로 (0 성공) */
int counter_destroy(counter_t *c)
{
	(void)c;
	/* TODO */
	return -1;
}
