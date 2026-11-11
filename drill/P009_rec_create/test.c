#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "drill_test.h"

/* ── malloc/free 를 가로채 호출 횟수·실패 주입·생존 개수를 센다 ── */
static int g_calls;   /* 무장 이후 malloc 호출 횟수 */
static int g_fail_at; /* N 번째 malloc 을 실패시킨다 (0 = 실패 없음) */
static int g_live;    /* 아직 free 되지 않은 블록 수 */

__attribute__((unused)) static void *test_malloc(size_t n)
{
	void *p;
	g_calls++;
	if (g_fail_at != 0 && g_calls == g_fail_at) return NULL;
	p = malloc(n);
	if (p) g_live++;
	return p;
}

__attribute__((unused)) static void test_free(void *p)
{
	if (p) g_live--;
	free(p);
}

static void arm(int fail_at)
{
	g_calls = 0;
	g_fail_at = fail_at;
	g_live = 0;
}

#define malloc(n) test_malloc(n)
#define free(p) test_free(p)

#include "solution.c"

int main(void)
{
	rec_t *r;
	rec_t dummy;
	rec_t *sentinel = &dummy;
	size_t i;
	int zero;
	int k;

	/* 정상 */
	arm(0);
	r = NULL;
	CHECK(rec_create(&r, "alice", 3) == 0);
	CHECK(r != NULL);
	if (r) {
		CHECK(r->name != NULL && strcmp(r->name, "alice") == 0);
		CHECK(r->nvals == 3);
		zero = (r->vals != NULL);
		for (i = 0; zero && i < 3; i++)
			if (r->vals[i] != 0) zero = 0;
		CHECK(zero);
		CHECK(g_calls == 3);   /* rec, name, vals — 정확히 세 번 */
		CHECK(g_live == 3);
		rec_destroy(r);
	}
	CHECK(g_live == 0);

	/* 핵심: k 번째 할당이 실패해도 앞서 할당한 것을 모두 되돌린다 (early return 누수) */
	for (k = 1; k <= 3; k++) {
		arm(k);
		r = sentinel;
		CHECK(rec_create(&r, "bob", 2) == -1);
		CHECK(r == sentinel); /* 실패 시 *out 은 그대로 */
		CHECK(g_live == 0);   /* 누수 0 */
	}

	/* 인자 오류: 할당 전에 거절, 누수 없음 */
	arm(0);
	r = sentinel;
	CHECK(rec_create(NULL, "x", 1) == -1);
	CHECK(rec_create(&r, NULL, 1) == -1);
	CHECK(rec_create(&r, "x", 0) == -1);
	CHECK(rec_create(&r, "x", 1025) == -1);
	CHECK(r == sentinel);
	CHECK(g_live == 0);

	/* 상한 경계 */
	arm(0);
	r = NULL;
	CHECK(rec_create(&r, "", 1024) == 0);
	if (r) {
		CHECK(r->name != NULL && r->name[0] == '\0');
		CHECK(r->nvals == 1024);
		rec_destroy(r);
	}
	CHECK(g_live == 0);

	/* destroy 방어 */
	rec_destroy(NULL);

	return drill_report();
}
