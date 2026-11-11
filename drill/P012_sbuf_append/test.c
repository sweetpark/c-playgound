#include <stdlib.h>
#include <string.h>

#include "drill_test.h"

/* realloc 을 가로채 N 번째 호출을 실패시킨다 */
static int g_rcalls;
static int g_rfail;

__attribute__((unused)) static void *test_realloc(void *p, size_t n)
{
	g_rcalls++;
	if (g_rfail != 0 && g_rcalls == g_rfail) return NULL;
	return realloc(p, n);
}

#define realloc(p, n) test_realloc((p), (n))

#include "solution.c"

static int is(const sbuf_t *b, const char *want)
{
	return b != NULL && b->len == strlen(want) && b->cap >= b->len + 1 &&
	       memcmp(b->data, want, b->len + 1) == 0;
}

int main(void)
{
	sbuf_t *b;
	sbuf_t *old;
	char big[80];
	int i;

	g_rcalls = 0;
	g_rfail = 0;

	/* 새 버퍼는 빈 문자열 */
	b = sbuf_new(4);
	CHECK(b != NULL);
	if (b) {
		CHECK(b->len == 0 && b->cap == 4);
		CHECK(b->data[0] == '\0');

		/* 안 늘려도 되는 경우: len 2 + 1 <= 4 */
		CHECK(sbuf_append(&b, "ab") == 0);
		CHECK(is(b, "ab"));
		CHECK(g_rcalls == 0);

		/* 늘려야 하는 경우: realloc 이 주소를 바꿔도 호출자 포인터가 갱신돼야 한다 */
		CHECK(sbuf_append(&b, "cdef") == 0);
		CHECK(is(b, "abcdef"));
		CHECK(g_rcalls == 1);

		/* 이어 붙일 위치는 cap 이 아니라 현재 길이 */
		CHECK(sbuf_append(&b, "!") == 0);
		CHECK(is(b, "abcdef!"));
		CHECK(sbuf_append(&b, "") == 0);
		CHECK(is(b, "abcdef!"));
		free(b);
	}

	/* realloc 실패: 원본은 그대로 유효해야 한다 (b = realloc(b, ..) 로 바로 받으면 여기서 누수/소실) */
	b = sbuf_new(4);
	CHECK(b != NULL);
	if (b) {
		CHECK(sbuf_append(&b, "xy") == 0);
		old = b;
		g_rcalls = 0;
		g_rfail = 1;
		CHECK(sbuf_append(&b, "0123456789") == -1);
		g_rfail = 0;
		CHECK(b == old);
		CHECK(is(b, "xy"));
		CHECK(b->cap == 4);
		free(b);
	}

	/* 여러 번 이어 붙이기 */
	b = sbuf_new(1);
	CHECK(b != NULL);
	if (b) {
		for (i = 0; i < 10; i++) CHECK(sbuf_append(&b, "xyz") == 0);
		CHECK(b->len == 30);
		CHECK(is(b, "xyzxyzxyzxyzxyzxyzxyzxyzxyzxyz"));
		free(b);
	}

	/* 상한: 널 포함 64 바이트. 63 글자는 되고 64 글자는 거절 */
	memset(big, 'a', sizeof(big));
	b = sbuf_new(8);
	CHECK(b != NULL);
	if (b) {
		big[63] = '\0';
		CHECK(sbuf_append(&b, big) == 0);
		CHECK(b->len == 63);
		CHECK(sbuf_append(&b, "z") == -1);
		CHECK(b->len == 63 && b->data[63] == '\0');
		free(b);
	}
	b = sbuf_new(8);
	if (b) {
		big[63] = 'a';
		big[64] = '\0';
		CHECK(sbuf_append(&b, big) == -1);
		CHECK(is(b, ""));
		free(b);
	}

	/* 인자 오류, 생성 인자 */
	b = sbuf_new(4);
	if (b) {
		CHECK(sbuf_append(&b, NULL) == -1);
		CHECK(sbuf_append(NULL, "x") == -1);
		free(b);
	}
	{
		sbuf_t *nul = NULL;
		CHECK(sbuf_append(&nul, "x") == -1);
	}
	CHECK(sbuf_new(0) == NULL);
	CHECK(sbuf_new(65) == NULL);

	return drill_report();
}
