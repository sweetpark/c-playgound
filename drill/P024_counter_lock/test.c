#include <errno.h>
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

#define THREADS 4
#define LOOPS 200000

static void *worker(void *arg)
{
	counter_t *c = arg;
	int i;
	for (i = 0; i < LOOPS; i++) counter_add(c, 1);
	return NULL;
}

static void *plusminus(void *arg)
{
	counter_t *c = arg;
	int i;
	for (i = 0; i < LOOPS; i++) {
		counter_add(c, 3);
		counter_add(c, -3);
	}
	return NULL;
}

int main(void)
{
	counter_t c;
	pthread_t th[THREADS + 1];
	int i;

	signal(SIGALRM, on_alarm);
	alarm(60);

	if (counter_init(&c) != 0) {
		CHECK(0 && "counter_init failed");
		return drill_report();   /* 초기화 안 된 뮤텍스를 만지지 않도록 중단 */
	}
	CHECK(counter_get(&c) == 0);

	/* 개발 중에는 ERRORCHECK 뮤텍스: 잡지 않은 뮤텍스를 푸는 실수를 EPERM 으로 알려 준다 */
	CHECK(pthread_mutex_unlock(&c.mu) == EPERM);

	counter_add(&c, 5);
	counter_add(&c, -2);
	CHECK(counter_get(&c) == 3);

	/* 여러 스레드가 같은 변수를 갱신: 모든 접근 경로가 같은 락 */
	counter_add(&c, -3);
	for (i = 0; i < THREADS; i++) pthread_create(&th[i], NULL, worker, &c);
	pthread_create(&th[THREADS], NULL, plusminus, &c);
	for (i = 0; i <= THREADS; i++) pthread_join(th[i], NULL);
	CHECK(counter_get(&c) == (long)THREADS * LOOPS);

	CHECK(counter_destroy(&c) == 0);   /* 아무도 잡고 있지 않아야 0 (EBUSY 면 unlock 누락) */
	return drill_report();
}
