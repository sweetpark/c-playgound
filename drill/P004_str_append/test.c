#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

#define PAD 0x7E

/* cap 바이트 힙 버퍼를 만들고 init 을 (널 포함) 넣는다. 나머지는 PAD. init 은 cap 에 들어가야 한다 */
static char *mk(size_t cap, const char *init)
{
	char *p = malloc(cap);
	if (!p) return NULL;
	memset(p, PAD, cap);
	if (init) memcpy(p, init, strlen(init) + 1);
	return p;
}

static int eq(const char *buf, size_t cap, const char *expect)
{
	return memchr(buf, '\0', cap) != NULL && strcmp(buf, expect) == 0;
}

static void run(size_t cap, const char *init, const char *src, int want_ret, const char *want)
{
	char *p = mk(cap, init);
	int r;
	CHECK(p != NULL);
	if (!p) return;
	r = str_append(p, cap, src);
	CHECK(r == want_ret);
	CHECK(eq(p, cap, want));
	free(p);
}

int main(void)
{
	char *p;
	size_t i;
	int same;
	int r;

	run(10, "dst", "src", 0, "dstsrc");
	run(7, "abc", "def", 0, "abcdef"); /* 딱 맞음: dlen + slen + 1 == size */
	run(6, "abc", "def", -1, "abc");   /* 1 부족: dst 변화 없음 */
	run(4, "", "xyz", 0, "xyz");
	run(8, "abc", "", 0, "abc");
	run(1, "", "", 0, "");
	run(1, "", "a", -1, "");
	run(5, "abcd", "x", -1, "abcd");   /* dst 가 꽉 찬 문자열 */

	/* 이어 붙이기를 여러 번 */
	p = mk(8, "ab");
	CHECK(p != NULL);
	if (p) {
		CHECK(str_append(p, 8, "cd") == 0);
		CHECK(str_append(p, 8, "ef") == 0);
		CHECK(eq(p, 8, "abcdef"));
		CHECK(str_append(p, 8, "gh") == -1); /* 6 + 2 + 1 = 9 > 8 */
		CHECK(eq(p, 8, "abcdef"));
		CHECK(str_append(p, 8, "g") == 0);   /* 6 + 1 + 1 = 8 */
		CHECK(eq(p, 8, "abcdefg"));
		free(p);
	}

	/* 널 종료 누락: dst 에서 strlen 을 부르면 범위 밖을 읽는다 (ASan) */
	p = malloc(8);
	CHECK(p != NULL);
	if (p) {
		memset(p, 'A', 8);
		r = str_append(p, 8, "x");
		CHECK(r == -1);
		same = 1;
		for (i = 0; i < 8; i++)
			if (p[i] != 'A') same = 0;
		CHECK(same);
		free(p);
	}

	/* 인자 오류 */
	p = mk(8, "abc");
	CHECK(p != NULL);
	if (p) {
		CHECK(str_append(p, 8, NULL) == -1);
		CHECK(str_append(p, 0, "x") == -1);
		CHECK(str_append(NULL, 8, "x") == -1);
		CHECK(eq(p, 8, "abc"));
		free(p);
	}

	return drill_report();
}
