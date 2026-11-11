#include <string.h>

#include "drill_test.h"
#include "solution.c"

int main(void)
{
	const char *names[8];
	const char *few[2];
	size_t n, i;
	int ok;

	/* 이름 */
	CHECK(strcmp(level_name(LV_DEBUG), "DEBUG") == 0);
	CHECK(strcmp(level_name(LV_INFO), "INFO") == 0);
	CHECK(strcmp(level_name(LV_WARN), "WARN") == 0);
	CHECK(strcmp(level_name(LV_ERROR), "ERROR") == 0);
	/* 범위 밖: 모든 경로에서 값을 돌려준다 (return 없이 끝나면 쓰레기) */
	CHECK(strcmp(level_name(LV_COUNT), "UNKNOWN") == 0);
	CHECK(strcmp(level_name(-1), "UNKNOWN") == 0);
	CHECK(strcmp(level_name(99), "UNKNOWN") == 0);

	/* 파싱: 대소문자 무시, 정확히 일치해야 함 */
	CHECK(level_parse("DEBUG") == LV_DEBUG);
	CHECK(level_parse("info") == LV_INFO);
	CHECK(level_parse("WaRn") == LV_WARN);
	CHECK(level_parse("error") == LV_ERROR);
	CHECK(level_parse("INFOX") == -1);
	CHECK(level_parse("INF") == -1);
	CHECK(level_parse("") == -1);
	CHECK(level_parse(NULL) == -1);

	/* 전체 목록: 개수는 LV_COUNT, 순회는 size_t 가 아니라 enum/int 범위 안 */
	n = level_names(names, 8);
	CHECK(n == (size_t)LV_COUNT);
	ok = (n == (size_t)LV_COUNT);
	for (i = 0; ok && i < n; i++)
		if (names[i] == NULL || strcmp(names[i], level_name((int)i)) != 0) ok = 0;
	CHECK(ok);

	/* 칸이 모자라면 아무것도 채우지 않고 0 */
	few[0] = few[1] = NULL;
	CHECK(level_names(few, 2) == 0);
	CHECK(few[0] == NULL && few[1] == NULL);
	CHECK(level_names(NULL, 8) == 0);

	/* 왕복 */
	for (i = 0; i < (size_t)LV_COUNT; i++)
		if (level_parse(level_name((int)i)) != (int)i) { CHECK(0); break; }

	return drill_report();
}
