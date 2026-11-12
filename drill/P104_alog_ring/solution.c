/* P104 alog — 요구사항은 problem.md. alog.h 의 선언을 보고 구조체와 함수를 채운다. */
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "alog.h"

struct alog {
	/* TODO: 큐(링 버퍼), 카운터, FULL 상태, 뮤텍스/조건변수, 소비 스레드를 담을 멤버를 정한다 */
	int unused;
};

alog_t *alog_create(int capacity, alog_sink_fn sink, void *ctx)
{
	(void)capacity;
	(void)sink;
	(void)ctx;
	/* TODO */
	return NULL;
}

int alog_write(alog_t *a, const char *line)
{
	(void)a;
	(void)line;
	/* TODO */
	return ALOG_BADARG;
}

int alog_is_full(alog_t *a)
{
	(void)a;
	/* TODO */
	return 0;
}

void alog_stats(alog_t *a, unsigned long *enq, unsigned long *deq, unsigned long *dropped, int *used)
{
	(void)a;
	(void)enq;
	(void)deq;
	(void)dropped;
	(void)used;
	/* TODO */
}

void alog_destroy(alog_t *a)
{
	(void)a;
	/* TODO */
}
