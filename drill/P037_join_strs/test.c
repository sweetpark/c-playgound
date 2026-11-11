#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

#define PAD 0x7E

static int eq(const char *buf, size_t cap, const char *expect)
{
	return memchr(buf, '\0', cap) != NULL && strcmp(buf, expect) == 0;
}

/* 정확히 cap 바이트 힙 버퍼에서 호출 */
static int call(size_t cap, const char *const *items, size_t cnt, const char *sep, const char *want_out)
{
	char *h = malloc(cap);
	int r;
	int ok;
	if (!h) return -100;
	memset(h, PAD, cap);
	r = join_strs(h, cap, items, cnt, sep);
	ok = want_out ? eq(h, cap, want_out) : ((unsigned char)h[0] == PAD);
	free(h);
	return ok ? r : -101;
}

int main(void)
{
	const char *abc[] = {"a", "b", "c"};
	const char *one[] = {"solo"};
	const char *has_null[] = {"a", NULL, "c"};
	const char *emp[] = {"", "x", ""};

	CHECK(call(32, abc, 3, ", ", "a, b, c") == 0);
	CHECK(call(32, abc, 3, "", "abc") == 0);
	CHECK(call(32, one, 1, ", ", "solo") == 0);       /* 구분자는 사이에만 */
	CHECK(call(32, abc, 0, ", ", "") == 0);           /* 0 개: 빈 문자열 */
	CHECK(call(32, emp, 3, "-", "-x-") == 0);

	/* 경계: "a, b, c" 는 7 글자 → 8 바이트면 딱 맞음, 7 바이트면 잘림 */
	CHECK(call(8, abc, 3, ", ", "a, b, c") == 0);
	CHECK(call(7, abc, 3, ", ", "") == -2);           /* 잘리면 오류 + 빈 문자열 */
	CHECK(call(1, abc, 3, ", ", "") == -2);
	CHECK(call(1, abc, 0, ", ", "") == 0);
	/* 앞쪽에서는 들어가고 뒤에서 모자라는 경우 (sprintf 반환값 누적이 틀리는 지점) */
	CHECK(call(4, abc, 3, ", ", "") == -2);
	CHECK(call(5, abc, 3, ", ", "") == -2);

	/* 인자 오류: out 을 건드리지 않는다 */
	CHECK(call(16, has_null, 3, ",", NULL) == -1);
	CHECK(call(16, abc, 3, NULL, NULL) == -1);
	CHECK(call(16, NULL, 3, ",", NULL) == -1);
	{
		char b[8];
		memset(b, PAD, sizeof(b));
		CHECK(join_strs(NULL, 8, abc, 3, ",") == -1);
		CHECK(join_strs(b, 0, abc, 3, ",") == -1);
		CHECK((unsigned char)b[0] == PAD);
	}
	return drill_report();
}
