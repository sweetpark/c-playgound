/* P104 alog — 모범답안 */
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "alog.h"

struct alog {
	char **q;               /* 링 버퍼: 줄을 가리키는 포인터 capacity 칸 */
	int cap, head, count;   /* head = 다음에 꺼낼 칸, count = 쌓인 줄 수 */
	int full;               /* "가득 참" 상태 (히스테리시스) */
	int stop;               /* destroy 요청 */
	unsigned long enq, deq, dropped;
	alog_sink_fn sink;
	void *ctx;
	pthread_mutex_t mu;
	pthread_cond_t cv;
	pthread_t th;
};

/* 빈 칸이 이 값 이상이 되어야 FULL 을 끈다 */
static int release_slots(const alog_t *a)
{
	int n = a->cap / 10;
	return n < 1 ? 1 : n;
}

static void *consumer(void *arg)
{
	alog_t *a = arg;

	for (;;) {
		char *line;

		pthread_mutex_lock(&a->mu);
		while (a->count == 0 && !a->stop)
			pthread_cond_wait(&a->cv, &a->mu);
		if (a->count == 0) {                 /* stop 이고 다 비웠다 → 종료 */
			pthread_mutex_unlock(&a->mu);
			break;
		}
		line = a->q[a->head];
		a->q[a->head] = NULL;
		a->head = (a->head + 1) % a->cap;
		a->count--;
		a->deq++;
		if (a->full && a->cap - a->count >= release_slots(a))
			a->full = 0;
		pthread_mutex_unlock(&a->mu);

		a->sink(a->ctx, line);               /* 느린 일은 락 밖에서 */
		free(line);
	}
	return NULL;
}

alog_t *alog_create(int capacity, alog_sink_fn sink, void *ctx)
{
	alog_t *a;

	if (capacity <= 0 || sink == NULL)
		return NULL;
	a = calloc(1, sizeof(*a));
	if (a == NULL)
		return NULL;
	a->q = calloc((size_t)capacity, sizeof(char *));
	if (a->q == NULL) {
		free(a);
		return NULL;
	}
	a->cap = capacity;
	a->sink = sink;
	a->ctx = ctx;
	pthread_mutex_init(&a->mu, NULL);
	pthread_cond_init(&a->cv, NULL);
	if (pthread_create(&a->th, NULL, consumer, a) != 0) {
		pthread_cond_destroy(&a->cv);
		pthread_mutex_destroy(&a->mu);
		free(a->q);
		free(a);
		return NULL;
	}
	return a;
}

int alog_write(alog_t *a, const char *line)
{
	char *copy;
	int tail;

	if (a == NULL || line == NULL)
		return ALOG_BADARG;
	copy = strdup(line);                     /* 호출자의 버퍼는 곧 바뀐다 → 복사해 둔다 */
	if (copy == NULL)
		return ALOG_BADARG;

	pthread_mutex_lock(&a->mu);
	if (a->count == a->cap) {                /* 가득 참: 기다리지 않고 버린다 */
		a->dropped++;
		a->full = 1;
		pthread_mutex_unlock(&a->mu);
		free(copy);
		return ALOG_FULL;
	}
	tail = (a->head + a->count) % a->cap;
	a->q[tail] = copy;
	a->count++;
	a->enq++;
	if (a->count == a->cap)
		a->full = 1;
	pthread_cond_signal(&a->cv);
	pthread_mutex_unlock(&a->mu);
	return ALOG_OK;
}

int alog_is_full(alog_t *a)
{
	int f;

	if (a == NULL)
		return 0;
	pthread_mutex_lock(&a->mu);
	f = a->full;
	pthread_mutex_unlock(&a->mu);
	return f;
}

void alog_stats(alog_t *a, unsigned long *enq, unsigned long *deq, unsigned long *dropped, int *used)
{
	if (a == NULL)
		return;
	pthread_mutex_lock(&a->mu);
	if (enq)     *enq = a->enq;
	if (deq)     *deq = a->deq;
	if (dropped) *dropped = a->dropped;
	if (used)    *used = a->count;
	pthread_mutex_unlock(&a->mu);
}

void alog_destroy(alog_t *a)
{
	if (a == NULL)
		return;
	pthread_mutex_lock(&a->mu);
	a->stop = 1;
	pthread_cond_broadcast(&a->cv);
	pthread_mutex_unlock(&a->mu);
	pthread_join(a->th, NULL);               /* 소비 스레드가 남은 줄을 다 내보내고 끝난다 */
	pthread_cond_destroy(&a->cv);
	pthread_mutex_destroy(&a->mu);
	free(a->q);
	free(a);
}
