#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "alog.h"
#include "drill_test.h"
#include "solution.c"

/* ── 시험용 sink: 줄을 기록하고, "문"이 닫혀 있으면 허가가 날 때까지 기다린다 (느린 디스크 흉내) ── */
#define MAXL 8192
static pthread_mutex_t g_mu = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cv = PTHREAD_COND_INITIALIZER;
static char g_lines[MAXL][32];
static int g_nlines;
static int g_open;        /* 1 이면 기다리지 않는다 */
static int g_permits;     /* 문이 닫힌 동안 한 줄씩 통과시키는 허가 수 */

static void sink(void *ctx, const char *line)
{
	(void)ctx;
	pthread_mutex_lock(&g_mu);
	while (!g_open && g_permits <= 0)
		pthread_cond_wait(&g_cv, &g_mu);
	if (!g_open)
		g_permits--;
	if (g_nlines < MAXL) {
		strncpy(g_lines[g_nlines], line, sizeof(g_lines[0]) - 1);
		g_lines[g_nlines][sizeof(g_lines[0]) - 1] = '\0';
		g_nlines++;
	}
	pthread_mutex_unlock(&g_mu);
}

static void gate_reset(int open)
{
	pthread_mutex_lock(&g_mu);
	g_nlines = 0;
	g_open = open;
	g_permits = 0;
	pthread_mutex_unlock(&g_mu);
}

static void gate_open(void)
{
	pthread_mutex_lock(&g_mu);
	g_open = 1;
	pthread_cond_broadcast(&g_cv);
	pthread_mutex_unlock(&g_mu);
}

static void gate_permit(int n)
{
	pthread_mutex_lock(&g_mu);
	g_permits += n;
	pthread_cond_broadcast(&g_cv);
	pthread_mutex_unlock(&g_mu);
}

static int nlines(void)
{
	int n;
	pthread_mutex_lock(&g_mu);
	n = g_nlines;
	pthread_mutex_unlock(&g_mu);
	return n;
}

/* deq 가 want 이상이 될 때까지 최대 2초 기다린다 */
static int wait_deq(alog_t *a, unsigned long want)
{
	struct timespec ts = {0, 5 * 1000 * 1000};
	unsigned long deq = 0;
	int i;

	for (i = 0; i < 400; i++) {
		alog_stats(a, NULL, &deq, NULL, NULL);
		if (deq >= want)
			return 1;
		nanosleep(&ts, NULL);
	}
	return 0;
}

static long elapsed_ms(const struct timespec *t0)
{
	struct timespec t1;
	clock_gettime(CLOCK_MONOTONIC, &t1);
	return (t1.tv_sec - t0->tv_sec) * 1000 + (t1.tv_nsec - t0->tv_nsec) / 1000000;
}

/* ── 다중 생산자 ── */
struct prod { alog_t *a; int id; };

static void *producer(void *arg)
{
	struct prod *p = arg;
	char buf[32];
	int i;

	for (i = 0; i < 1000; i++) {
		snprintf(buf, sizeof(buf), "t%d-%04d", p->id, i);
		alog_write(p->a, buf);
	}
	return NULL;
}

int main(void)
{
	alog_t *a;
	char buf[32];
	int i;

	/* [1] 만들기/잘못된 인자 */
	CHECK(alog_create(0, sink, NULL) == NULL);
	CHECK(alog_create(-5, sink, NULL) == NULL);
	CHECK(alog_create(8, NULL, NULL) == NULL);
	gate_reset(1);
	a = alog_create(8, sink, NULL);
	CHECK(a != NULL);
	if (a == NULL)
		return drill_report();
	CHECK(alog_write(NULL, "x") == ALOG_BADARG);
	CHECK(alog_write(a, NULL) == ALOG_BADARG);
	alog_destroy(a);
	alog_destroy(NULL);          /* NULL 은 조용히 무시 */

	/* [2] 기본: 쓴 순서대로 전부 나가고 destroy 가 끝나면 다 내보낸 상태 */
	gate_reset(1);
	a = alog_create(8, sink, NULL);
	for (i = 0; i < 5; i++) {
		snprintf(buf, sizeof(buf), "line %d", i);
		CHECK(alog_write(a, buf) == ALOG_OK);
	}
	alog_destroy(a);
	CHECK(nlines() == 5);
	for (i = 0; i < 5 && i < nlines(); i++) {
		snprintf(buf, sizeof(buf), "line %d", i);
		CHECK(strcmp(g_lines[i], buf) == 0);
	}

	/* [3] 소비자가 막혔을 때 한꺼번에 쓰면: 기다리지 않고 버린다 */
	{
		int acc[100], nacc = 0, ndrop = 0, other = 0;
		unsigned long enq, dropped;
		int used;
		struct timespec t0;

		gate_reset(0);                       /* 문 닫힘 = 소비자가 첫 줄에서 멈춘다 */
		a = alog_create(10, sink, NULL);
		alog_write(a, "line 0");             /* 첫 줄은 소비자가 꺼내 sink 에서 멈추게 한다 */
		acc[nacc++] = 0;
		CHECK(wait_deq(a, 1));               /* 소비자가 첫 줄을 쥔 것을 확인한 뒤에 채운다 (안 그러면 부하에 따라 결과가 달라진다) */
		clock_gettime(CLOCK_MONOTONIC, &t0);
		for (i = 1; i < 100; i++) {
			int r;
			snprintf(buf, sizeof(buf), "line %d", i);
			r = alog_write(a, buf);
			if (r == ALOG_OK) acc[nacc++] = i;
			else if (r == ALOG_FULL) ndrop++;
			else other++;
		}
		CHECK(elapsed_ms(&t0) < 500);        /* 소비자가 막혀 있어도 write 는 막히지 않는다 */
		CHECK(other == 0);
		CHECK(nacc + ndrop == 100);
		CHECK(ndrop > 0);
		CHECK(nacc == 10 || nacc == 11);     /* 큐 10칸 (+ 소비자가 이미 꺼내 들고 있는 1줄) */
		CHECK(alog_is_full(a) == 1);
		alog_stats(a, &enq, NULL, &dropped, &used);
		CHECK(enq == (unsigned long)nacc);
		CHECK(dropped == (unsigned long)ndrop);
		CHECK(used <= 10);

		/* [4] 문을 열고 destroy: 받아 둔 줄은 전부, 받은 순서대로 나간다 */
		gate_open();
		alog_destroy(a);
		CHECK(nlines() == nacc);
		for (i = 0; i < nacc && i < nlines(); i++) {
			snprintf(buf, sizeof(buf), "line %d", acc[i]);
			CHECK(strcmp(g_lines[i], buf) == 0);
		}
	}

	/* [5] FULL 해제는 빈 칸이 capacity/10 이상 생겼을 때 (칸 하나로 바로 풀리지 않는다) */
	gate_reset(0);
	a = alog_create(20, sink, NULL);         /* 해제 기준 = 20/10 = 2칸 */
	alog_write(a, "line 0");
	CHECK(wait_deq(a, 1));                   /* 첫 줄은 소비자가 꺼내 sink 에서 멈춰 있다 (이후 큐 20칸이 전부 쓸 수 있다) */
	for (i = 1; i < 40; i++) {
		snprintf(buf, sizeof(buf), "line %d", i);
		alog_write(a, buf);
	}
	CHECK(alog_is_full(a) == 1);
	gate_permit(1);                          /* 첫 줄이 나가고 소비자가 둘째 줄을 꺼낸다 → 빈 칸 1 */
	CHECK(wait_deq(a, 2));
	CHECK(alog_is_full(a) == 1);             /* 빈 칸 1 < 2 : 아직 FULL */
	gate_permit(1);                          /* 셋째 줄을 꺼낸다 → 빈 칸 2 */
	CHECK(wait_deq(a, 3));
	CHECK(alog_is_full(a) == 0);             /* 빈 칸 2 >= 2 : FULL 해제 */
	CHECK(alog_write(a, "again") == ALOG_OK);
	gate_open();
	alog_destroy(a);

	/* [6] 생산자 4명이 동시에 써도 하나도 잃지 않고, 각자의 순서가 유지된다 */
	{
		pthread_t th[4];
		struct prod pr[4];
		int last[4] = {-1, -1, -1, -1}, ok = 1, t;

		gate_reset(1);
		a = alog_create(4096, sink, NULL);
		for (t = 0; t < 4; t++) {
			pr[t].a = a;
			pr[t].id = t;
			pthread_create(&th[t], NULL, producer, &pr[t]);
		}
		for (t = 0; t < 4; t++)
			pthread_join(th[t], NULL);
		alog_destroy(a);
		CHECK(nlines() == 4000);
		for (i = 0; i < nlines(); i++) {
			int id, seq;
			if (sscanf(g_lines[i], "t%d-%d", &id, &seq) != 2 || id < 0 || id > 3 || seq != last[id] + 1) {
				ok = 0;
				break;
			}
			last[id] = seq;
		}
		CHECK(ok);
	}

	return drill_report();
}
