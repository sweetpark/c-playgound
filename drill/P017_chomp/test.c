#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

/* 정확히 strlen+1 바이트 힙에 복사해서 부른다 (범위 밖 접근은 ASan) */
static void run(const char *in, const char *want)
{
	size_t n = strlen(in) + 1;
	char *h = malloc(n);
	size_t r;
	if (!h) { CHECK(h != NULL); return; }
	memcpy(h, in, n);
	r = chomp(h);
	CHECK(r == strlen(want));
	CHECK(strcmp(h, want) == 0);
	free(h);
}

int main(void)
{
	run("abc\n", "abc");
	run("abc", "abc");          /* 개행 없음: 마지막 글자를 지우면 안 된다 (fgets 가 잘라 온 줄) */
	run("", "");                /* 길이 0: s[strlen-1] 은 s[-1] */
	run("\n", "");
	run("abc\r\n", "abc");
	run("a\r", "a\r");          /* CR 만으로는 줄 끝이 아니다 */
	run("ab\n\n", "ab\n");      /* 끝의 하나만 */
	run("12345678", "12345678");

	CHECK(chomp(NULL) == 0);
	return drill_report();
}
