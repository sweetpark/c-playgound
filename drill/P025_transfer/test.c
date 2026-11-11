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

#define LOOPS 100000

typedef struct {
	acct_t *from, *to;
	int ok;
} job_t;

static void *runner(void *arg)
{
	job_t *j = arg;
	int i;
	for (i = 0; i < LOOPS; i++)
		if (transfer(j->from, j->to, 1) == 0) j->ok++;
	return NULL;
}

int main(void)
{
	acct_t a, b;
	pthread_t t1, t2, t3, t4;
	job_t j1, j2, j3, j4;

	signal(SIGALRM, on_alarm);
	alarm(60);

	acct_init(&a, 1, 1000000);
	acct_init(&b, 2, 1000000);

	/* 기본 동작 */
	CHECK(transfer(&a, &b, 300) == 0);
	CHECK(a.bal == 999700 && b.bal == 1000300);

	/* 거절: 잔액 부족 / 금액 <= 0 / 같은 계좌(자기 자신에게 두 번 락 잡지 않는다) / NULL */
	CHECK(transfer(&a, &b, 5000000) == -1);
	CHECK(a.bal == 999700 && b.bal == 1000300);
	CHECK(transfer(&a, &b, 0) == -1);
	CHECK(transfer(&a, &b, -5) == -1);
	CHECK(transfer(&a, &a, 10) == -1);
	CHECK(transfer(NULL, &b, 10) == -1);
	CHECK(transfer(&a, NULL, 10) == -1);

	/* 핵심: 서로 반대 방향 이체를 동시에 — 락 순서가 고정되지 않으면 교착 */
	j1.from = &a; j1.to = &b; j1.ok = 0;
	j2.from = &b; j2.to = &a; j2.ok = 0;
	j3.from = &a; j3.to = &b; j3.ok = 0;
	j4.from = &b; j4.to = &a; j4.ok = 0;
	pthread_create(&t1, NULL, runner, &j1);
	pthread_create(&t2, NULL, runner, &j2);
	pthread_create(&t3, NULL, runner, &j3);
	pthread_create(&t4, NULL, runner, &j4);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);

	CHECK(j1.ok == LOOPS && j2.ok == LOOPS && j3.ok == LOOPS && j4.ok == LOOPS);
	CHECK(a.bal + b.bal == 2000000);                    /* 총합 보존 */
	CHECK(a.bal == 999700 && b.bal == 1000300);         /* 양방향 횟수가 같으므로 원래 값 */

	return drill_report();
}
