#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

#define PAD 0x7E

/* 정확히 cap 바이트 힙 버퍼로 호출 (넘치면 ASan) */
static int call(const char *name, size_t cap, char **keep)
{
	char *h = malloc(cap);
	int r;
	if (!h) return -99;
	memset(h, PAD, cap);
	r = env_copy(name, h, cap);
	*keep = h;
	return r;
}

static int untouched(const char *h, size_t cap)
{
	size_t i;
	for (i = 0; i < cap; i++)
		if ((unsigned char)h[i] != PAD) return 0;
	return 1;
}

int main(void)
{
	char *h;
	char big[200];

	setenv("DRILL_T", "value", 1);
	unsetenv("DRILL_UNSET");
	setenv("DRILL_EMPTY", "", 1);
	memset(big, 'L', 150);
	big[150] = '\0';
	setenv("DRILL_BIG", big, 1);

	/* 정상 + 딱 맞음("value" 5자 + NUL = 6) */
	CHECK(call("DRILL_T", 6, &h) == 0);
	CHECK(memchr(h, 0, 6) && strcmp(h, "value") == 0);
	free(h);
	CHECK(call("DRILL_T", 64, &h) == 0);
	CHECK(strcmp(h, "value") == 0);
	free(h);

	/* 1 부족: 잘라 넣지 않고 오류. 외부 값은 길이를 믿을 수 없다 */
	CHECK(call("DRILL_T", 5, &h) == -2);
	CHECK(untouched(h, 5));
	free(h);

	/* 150자 환경변수를 16바이트 버퍼에 (strcpy 라면 넘친다) */
	CHECK(call("DRILL_BIG", 16, &h) == -2);
	CHECK(untouched(h, 16));
	free(h);

	/* 빈 값은 설정되어 있는 것: 0 + "" ; 미설정은 -1 */
	CHECK(call("DRILL_EMPTY", 4, &h) == 0);
	CHECK(h[0] == '\0');
	free(h);
	CHECK(call("DRILL_UNSET", 8, &h) == -1);
	CHECK(untouched(h, 8));
	free(h);

	/* 반환된 사본을 바꿔도 환경은 그대로 (getenv 포인터를 직접 수정하지 않는다) */
	CHECK(call("DRILL_T", 16, &h) == 0);
	h[0] = 'X';
	free(h);
	CHECK(strcmp(getenv("DRILL_T"), "value") == 0);

	/* 인자 오류 */
	{
		char b[8];
		memset(b, PAD, sizeof(b));
		CHECK(env_copy(NULL, b, sizeof(b)) == -1);
		CHECK(env_copy("DRILL_T", NULL, 8) == -1);
		CHECK(env_copy("DRILL_T", b, 0) == -1);
		CHECK(untouched(b, sizeof(b)));
	}
	return drill_report();
}
