#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

#define PAD 0x7E

typedef struct {
	unsigned char before[8];
	char buf[32];
	unsigned char after[8];
} guarded_t;

static void guard_init(guarded_t *g) { memset(g, PAD, sizeof(*g)); }

static int guard_ok(const guarded_t *g)
{
	size_t i;
	for (i = 0; i < sizeof(g->before); i++)
		if (g->before[i] != PAD || g->after[i] != PAD) return 0;
	return 1;
}

static int untouched(const guarded_t *g)
{
	size_t i;
	for (i = 0; i < sizeof(g->buf); i++)
		if ((unsigned char)g->buf[i] != PAD) return 0;
	return guard_ok(g);
}

static int eq(const char *buf, size_t cap, const char *expect)
{
	return memchr(buf, '\0', cap) != NULL && strcmp(buf, expect) == 0;
}

/* path 는 수정되면 안 된다: 복사본과 비교. (리터럴을 그대로 넘기면 쓰는 순간 SIGSEGV) */
static void ok_case(const char *path, const char *want)
{
	guarded_t g;
	char orig[64];
	char work[64];
	int r;

	strcpy(orig, path);
	strcpy(work, path);
	guard_init(&g);
	r = get_dirname(work, g.buf, sizeof(g.buf));
	CHECK(r == 0);
	CHECK(eq(g.buf, sizeof(g.buf), want));
	CHECK(strcmp(work, orig) == 0); /* 원본 불변 */
	CHECK(guard_ok(&g));
}

static void fail_case(const char *path)
{
	guarded_t g;
	guard_init(&g);
	CHECK(get_dirname(path, g.buf, sizeof(g.buf)) == -1);
	CHECK(untouched(&g));
}

/* out 이 정확히 cap 바이트인 힙 버퍼 */
static int heap_case(const char *path, size_t cap, const char *want)
{
	char *h = malloc(cap);
	int r, good;
	if (!h) return -2;
	memset(h, PAD, cap);
	r = get_dirname(path, h, cap);
	good = eq(h, cap, want);
	free(h);
	if (r == 0) return good ? 0 : -3;
	return r;
}

int main(void)
{
	guarded_t g;

	ok_case("/home/dir", "/home");
	ok_case("/home/dir/", "/home/dir");
	ok_case("/a/b/c.txt", "/a/b");
	ok_case("a/b", "a");
	ok_case("/x", "/");    /* 루트 바로 아래 */
	ok_case("/", "/");

	/* 리터럴(읽기 전용)을 직접 넘겨도 안전해야 한다 */
	guard_init(&g);
	CHECK(get_dirname("/home/dir", g.buf, sizeof(g.buf)) == 0);
	CHECK(eq(g.buf, sizeof(g.buf), "/home"));

	fail_case("file");     /* 슬래시 없음 */
	fail_case("");
	fail_case(NULL);

	/* 출력 버퍼 경계: "/home" 은 5자 + NUL = 6 */
	CHECK(heap_case("/home/dir", 6, "/home") == 0);
	CHECK(heap_case("/home/dir", 5, "") == -1);
	CHECK(heap_case("/x", 2, "/") == 0);
	CHECK(heap_case("/x", 1, "") == -1);

	/* 인자 오류 */
	guard_init(&g);
	CHECK(get_dirname("/a/b", g.buf, 0) == -1);
	CHECK(untouched(&g));
	CHECK(get_dirname("/a/b", NULL, 32) == -1);

	return drill_report();
}
