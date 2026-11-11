#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

#define PAD 0x7E

/* 앞뒤에 보호 바이트를 둔 버퍼: dstsize 밖을 건드리면 PAD 가 변한다 */
typedef struct {
	unsigned char before[8];
	char buf[8];
	unsigned char after[8];
} guarded_t;

static void guard_init(guarded_t *g)
{
	memset(g, PAD, sizeof(*g));
}

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

/* buf[0..cap) 안에 NUL 이 있고 내용이 expect 와 같은가.
 * NUL 이 없으면 범위 밖을 읽지 않고 그냥 false (널 종료 누락 = 실패). */
static int eq(const char *buf, size_t cap, const char *expect)
{
	return memchr(buf, '\0', cap) != NULL && strcmp(buf, expect) == 0;
}

int main(void)
{
	guarded_t g;
	int r;
	char *h;

	/* 정상: 짧은 문자열 */
	guard_init(&g);
	r = safe_copy(g.buf, sizeof(g.buf), "abc");
	CHECK(r == 0);
	CHECK(eq(g.buf, sizeof(g.buf), "abc"));
	CHECK(guard_ok(&g));

	/* 빈 문자열 */
	guard_init(&g);
	r = safe_copy(g.buf, sizeof(g.buf), "");
	CHECK(r == 0);
	CHECK(eq(g.buf, sizeof(g.buf), ""));
	CHECK(guard_ok(&g));

	/* 경계: 딱 맞는 길이(7자 + NUL) */
	guard_init(&g);
	r = safe_copy(g.buf, sizeof(g.buf), "1234567");
	CHECK(r == 0);
	CHECK(eq(g.buf, sizeof(g.buf), "1234567"));
	CHECK(guard_ok(&g));

	/* 함정: 8자 → strncpy 라면 널이 없다 */
	guard_init(&g);
	r = safe_copy(g.buf, sizeof(g.buf), "12345678");
	CHECK(r == 1);
	CHECK(eq(g.buf, sizeof(g.buf), "1234567"));
	CHECK(guard_ok(&g));

	/* 훨씬 긴 문자열 */
	guard_init(&g);
	r = safe_copy(g.buf, sizeof(g.buf), "this string is much longer than eight bytes");
	CHECK(r == 1);
	CHECK(eq(g.buf, sizeof(g.buf), "this st"));
	CHECK(guard_ok(&g));

	/* dstsize == 1: 빈 문자열만 들어간다 */
	guard_init(&g);
	r = safe_copy(g.buf, 1, "x");
	CHECK(r == 1);
	CHECK(g.buf[0] == '\0');
	CHECK((unsigned char)g.buf[1] == PAD);
	guard_init(&g);
	r = safe_copy(g.buf, 1, "");
	CHECK(r == 0);
	CHECK(g.buf[0] == '\0');

	/* 인자 오류: dst 를 건드리지 않는다 */
	guard_init(&g);
	r = safe_copy(g.buf, 0, "x");
	CHECK(r == -1);
	CHECK(untouched(&g));

	guard_init(&g);
	r = safe_copy(g.buf, sizeof(g.buf), NULL);
	CHECK(r == -1);
	CHECK(untouched(&g));

	r = safe_copy(NULL, 8, "x");
	CHECK(r == -1);

	/* 힙: 딱 맞는 크기 버퍼 — 1바이트라도 넘쳐 쓰면 ASan 이 잡는다 */
	h = malloc(8);
	CHECK(h != NULL);
	if (h) {
		memset(h, PAD, 8);
		r = safe_copy(h, 8, "12345678");
		CHECK(r == 1);
		CHECK(eq(h, 8, "1234567"));
		r = safe_copy(h, 8, "ok");
		CHECK(r == 0);
		CHECK(eq(h, 8, "ok"));
		free(h);
	}

	/* 원본이 const 리터럴이어도 안전해야 한다 */
	{
		const char *lit = "literal string, do not modify";
		char out[16];
		memset(out, PAD, sizeof(out));
		r = safe_copy(out, sizeof(out), lit);
		CHECK(r == 1);
		CHECK(eq(out, sizeof(out), "literal string,"));
	}

	return drill_report();
}
