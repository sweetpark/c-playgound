#include <pthread.h>
#include <stddef.h>

/* P026 bounded_queue — 요구사항은 problem.md */
#define BQ_CAP 8

typedef struct {
	pthread_mutex_t mu;
	pthread_cond_t not_empty;   /* 소비자가 기다리는 조건 */
	pthread_cond_t not_full;    /* 생산자가 기다리는 조건 */
	int buf[BQ_CAP];
	size_t head;                /* 다음에 꺼낼 위치 */
	size_t count;
	int closed;
} bq_t;

void bq_init(bq_t *q)
{
	(void)q;
	/* TODO */
}

void bq_destroy(bq_t *q)
{
	(void)q;
	/* TODO */
}

/* 가득 차면 빌 때까지 기다린다. 0 성공 / -1 닫힘 */
int bq_push(bq_t *q, int v)
{
	(void)q;
	(void)v;
	/* TODO */
	return -1;
}

/* 비어 있으면 올 때까지 기다린다. 0 성공 / -1 닫혔고 비었음 */
int bq_pop(bq_t *q, int *out)
{
	(void)q;
	(void)out;
	/* TODO */
	return -1;
}

/* 닫는다: 기다리는 모든 스레드를 깨운다 */
void bq_close(bq_t *q)
{
	(void)q;
	/* TODO */
}
