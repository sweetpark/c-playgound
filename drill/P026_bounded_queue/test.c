#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include "drill_test.h"
#include "solution.c"

static void on_alarm(int s)
{
	static const char m[] = "FAIL (timeout/deadlock)\n";
	(void)s;
	if (write(1, m, sizeof(m) - 1) < 0) { /* ignore */ }
	_exit(1);
}

#define N 5000

static void *producer(void *arg)
{
	bq_t *q = arg;
	int i;
	for (i = 1; i <= N; i++)
		if (bq_push(q, i) != 0) break;
	return NULL;
}

typedef struct {
	bq_t *q;
	long sum;
	long cnt;
} cons_t;

static void *consumer(void *arg)
{
	cons_t *c = arg;
	int v;
	while (bq_pop(c->q, &v) == 0) {
		c->sum += v;
		c->cnt++;
	}
	return NULL;
}

int main(void)
{
	bq_t q;
	pthread_t p1, p2, c1, c2, c3;
	cons_t k1 = {&q, 0, 0}, k2 = {&q, 0, 0}, k3 = {&q, 0, 0};
	int v = -1;

	signal(SIGALRM, on_alarm);
	alarm(60);

	bq_init(&q);

	/* 단일 스레드: FIFO, 용량(8)까지 */
	CHECK(bq_push(&q, 10) == 0);
	CHECK(bq_push(&q, 20) == 0);
	CHECK(bq_pop(&q, &v) == 0 && v == 10);
	CHECK(bq_pop(&q, &v) == 0 && v == 20);

	/* close: 남은 것은 모두 꺼낼 수 있고, 빈 뒤에는 -1, close 뒤 push 는 -1 */
	CHECK(bq_push(&q, 1) == 0);
	CHECK(bq_push(&q, 2) == 0);
	bq_close(&q);
	CHECK(bq_push(&q, 3) == -1);
	CHECK(bq_pop(&q, &v) == 0 && v == 1);
	CHECK(bq_pop(&q, &v) == 0 && v == 2);
	CHECK(bq_pop(&q, &v) == -1);
	bq_destroy(&q);

	/* 생산자 2 / 소비자 3: 가득 찬 큐에서 막히고 비면 기다리고, close 로 모두 깨어난다 */
	bq_init(&q);
	pthread_create(&c1, NULL, consumer, &k1);
	pthread_create(&c2, NULL, consumer, &k2);
	pthread_create(&c3, NULL, consumer, &k3);
	pthread_create(&p1, NULL, producer, &q);
	pthread_create(&p2, NULL, producer, &q);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	bq_close(&q);                       /* 대기 중인 소비자를 전부 깨운다 (signal 하나로는 부족) */
	pthread_join(c1, NULL);
	pthread_join(c2, NULL);
	pthread_join(c3, NULL);

	CHECK(k1.cnt + k2.cnt + k3.cnt == 2 * N);
	CHECK(k1.sum + k2.sum + k3.sum == 2L * N * (N + 1) / 2);
	bq_destroy(&q);

	return drill_report();
}
