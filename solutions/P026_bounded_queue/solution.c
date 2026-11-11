#include <pthread.h>
#include <stddef.h>

/* P026 bounded_queue — 모범답안 */
#define BQ_CAP 8

typedef struct {
	pthread_mutex_t mu;
	pthread_cond_t not_empty;
	pthread_cond_t not_full;
	int buf[BQ_CAP];
	size_t head;
	size_t count;
	int closed;
} bq_t;

void bq_init(bq_t *q)
{
	pthread_mutex_init(&q->mu, NULL);
	pthread_cond_init(&q->not_empty, NULL);
	pthread_cond_init(&q->not_full, NULL);
	q->head = 0;
	q->count = 0;
	q->closed = 0;
}

void bq_destroy(bq_t *q)
{
	pthread_cond_destroy(&q->not_full);
	pthread_cond_destroy(&q->not_empty);
	pthread_mutex_destroy(&q->mu);
}

int bq_push(bq_t *q, int v)
{
	pthread_mutex_lock(&q->mu);
	while (q->count == BQ_CAP && !q->closed)          /* if 가 아니라 while: 깨어난 뒤 조건 재확인 */
		pthread_cond_wait(&q->not_full, &q->mu);
	if (q->closed) {
		pthread_mutex_unlock(&q->mu);
		return -1;
	}
	q->buf[(q->head + q->count) % BQ_CAP] = v;
	q->count++;
	pthread_cond_signal(&q->not_empty);
	pthread_mutex_unlock(&q->mu);
	return 0;
}

int bq_pop(bq_t *q, int *out)
{
	pthread_mutex_lock(&q->mu);
	while (q->count == 0 && !q->closed)
		pthread_cond_wait(&q->not_empty, &q->mu);
	if (q->count == 0) {                              /* 닫혔고 비었음 */
		pthread_mutex_unlock(&q->mu);
		return -1;
	}
	*out = q->buf[q->head];
	q->head = (q->head + 1) % BQ_CAP;
	q->count--;
	pthread_cond_signal(&q->not_full);
	pthread_mutex_unlock(&q->mu);
	return 0;
}

void bq_close(bq_t *q)
{
	pthread_mutex_lock(&q->mu);
	q->closed = 1;
	pthread_cond_broadcast(&q->not_empty);            /* signal 은 대기자 하나만 깨운다 */
	pthread_cond_broadcast(&q->not_full);
	pthread_mutex_unlock(&q->mu);
}
