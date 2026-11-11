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

static void ok_case(const char *dir, const char *file, const char *want)
{
	guarded_t g;
	int r;
	guard_init(&g);
	r = build_path(g.buf, sizeof(g.buf), dir, file);
	CHECK(r == 0);
	CHECK(eq(g.buf, sizeof(g.buf), want));
	CHECK(guard_ok(&g));
}

/* 딱 cap 바이트 힙 버퍼에 만든다 — 넘치면 ASan.
 * 반환: 0 = (성공, 내용 일치) / -1 = (실패, out 이 "") / 그 외 = 기대와 다름 */
static int heap_case(size_t cap, const char *dir, const char *file, const char *want)
{
	char *h = malloc(cap);
	int r;
	int good;
	if (!h) return -2;
	memset(h, PAD, cap);
	r = build_path(h, cap, dir, file);
	good = eq(h, cap, want);
	free(h);
	if (r == 0) return good ? 0 : -3;
	return good ? -1 : -4;
}

int main(void)
{
	guarded_t g;
	size_t i;
	int pad;
	int r;

	ok_case("/home", "a.txt", "/home/a.txt");
	ok_case("/home/", "a.txt", "/home/a.txt");
	ok_case("", "a.txt", "a.txt"); /* "" 는 NULL 이 아니다 */
	ok_case("/", "a", "/a");
	ok_case("a//", "b", "a//b"); /* 정규화는 하지 않는다 */

	/* 딱 맞음 / 1 부족 (n >= outsize 경계) */
	CHECK(heap_case(5, "/h", "f", "/h/f") == 0);
	CHECK(heap_case(4, "/h", "f", "") == -1);
	CHECK(heap_case(6, "/h/", "f", "/h/f") == 0);
	CHECK(heap_case(1, "a", "b", "") == -1);

	/* 인자 오류: out 을 건드리지 않는다 */
	guard_init(&g);
	r = build_path(g.buf, sizeof(g.buf), NULL, "f");
	CHECK(r == -1);
	CHECK(untouched(&g));

	guard_init(&g);
	r = build_path(g.buf, sizeof(g.buf), "/d", NULL);
	CHECK(r == -1);
	CHECK(untouched(&g));

	guard_init(&g);
	r = build_path(g.buf, sizeof(g.buf), "/d", "");
	CHECK(r == -1);
	CHECK(untouched(&g));

	guard_init(&g);
	r = build_path(g.buf, 0, "/d", "f");
	CHECK(r == -1);
	CHECK(untouched(&g));

	r = build_path(NULL, 32, "/d", "f");
	CHECK(r == -1);

	/* 매우 긴 입력: 잘린 채 성공하지 않고, 실패 + 빈 문자열, outsize(16) 이후는 불변 */
	guard_init(&g);
	r = build_path(g.buf, 16, "/very/long/directory/name", "and_a_long_file_name.txt");
	CHECK(r == -1);
	CHECK(eq(g.buf, 16, ""));
	pad = 1;
	for (i = 16; i < sizeof(g.buf); i++)
		if ((unsigned char)g.buf[i] != PAD) pad = 0;
	CHECK(pad);
	CHECK(guard_ok(&g));

	return drill_report();
}
