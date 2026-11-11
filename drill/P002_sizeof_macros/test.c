#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

#define PAD 0x7E

/* 컴파일러가 문자열 길이를 상수로 알아 -Wformat-truncation 을 내지 않게 하는 우회 */
static const char *opaque(const char *s) { return s; }

typedef struct {
	unsigned char before[8];
	char buf[8];
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

static int eq(const char *buf, size_t cap, const char *expect)
{
	return memchr(buf, '\0', cap) != NULL && strcmp(buf, expect) == 0;
}

typedef struct {
	unsigned char before[8];
	int v[5];
	unsigned char after[8];
} ints_t;

typedef struct {
	unsigned char before[8];
	char c[13]; /* 홀수 크기 */
	unsigned char after[8];
} odd_t;

int main(void)
{
	int a7[7];
	long l3[3];
	char s16[16];
	struct { int a; char b; } st[4];
	int m[3][4];
	enum { N7 = ARRAY_LEN(a7) };
	guarded_t g;
	ints_t iv;
	odd_t od;
	double d3[3] = {1.5, 2.5, 3.5};
	const char *src = opaque("0123456789");
	int flag = 0;
	size_t i;

	/* ARRAY_LEN: 원소 개수 (바이트 수 아님) */
	CHECK(ARRAY_LEN(a7) == 7);
	CHECK(ARRAY_LEN(l3) == 3);
	CHECK(ARRAY_LEN(s16) == 16);
	CHECK(ARRAY_LEN(st) == 4);
	CHECK(ARRAY_LEN(m) == 3);
	CHECK(ARRAY_LEN(m[0]) == 4);
	CHECK(N7 == 7); /* 상수식 */
	CHECK(ARRAY_LEN(d3) == 3);
	(void)a7; (void)l3; (void)s16; (void)st; (void)m;

	/* COPY_STR: 잘림 + 널 종료 + 경계 보호 */
	guard_init(&g);
	COPY_STR(g.buf, opaque("0123456789"));
	CHECK(eq(g.buf, sizeof(g.buf), "0123456"));
	CHECK(guard_ok(&g));

	guard_init(&g);
	COPY_STR(g.buf, opaque("ab"));
	CHECK(eq(g.buf, sizeof(g.buf), "ab"));
	CHECK(guard_ok(&g));

	guard_init(&g);
	COPY_STR(g.buf, opaque("1234567")); /* 딱 맞는 길이 */
	CHECK(eq(g.buf, sizeof(g.buf), "1234567"));
	guard_init(&g);
	COPY_STR(g.buf, opaque("12345678")); /* 1 넘침 */
	CHECK(eq(g.buf, sizeof(g.buf), "1234567"));
	CHECK(guard_ok(&g));

	guard_init(&g);
	COPY_STR(g.buf, src); /* 변수 src */
	CHECK(eq(g.buf, sizeof(g.buf), "0123456"));
	CHECK(guard_ok(&g));

	memset(s16, PAD, sizeof(s16));
	COPY_STR(s16, opaque("hello"));
	CHECK(eq(s16, sizeof(s16), "hello"));

	/* if/else 사이에서도 한 문장으로 동작 */
	if (src != NULL)
		COPY_STR(g.buf, opaque("x"));
	else
		flag = 1;
	CHECK(flag == 0);
	CHECK(eq(g.buf, sizeof(g.buf), "x"));

	/* ZERO_ARRAY: 바이트 수 전체, 앞뒤는 그대로 */
	memset(&iv, 0xAA, sizeof(iv));
	ZERO_ARRAY(iv.v);
	for (i = 0; i < 5; i++) CHECK(iv.v[i] == 0);
	for (i = 0; i < 8; i++) CHECK(iv.before[i] == 0xAA && iv.after[i] == 0xAA);

	memset(&od, 0xAA, sizeof(od));
	ZERO_ARRAY(od.c);
	for (i = 0; i < sizeof(od.c); i++) CHECK(od.c[i] == 0);
	CHECK(od.before[7] == 0xAA && od.after[0] == 0xAA);

	ZERO_ARRAY(d3);
	CHECK(d3[0] == 0.0 && d3[1] == 0.0 && d3[2] == 0.0);

	return drill_report();
}
