#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

int main(void)
{
	int *p;
	size_t i, n;
	int all_zero;

	/* 정상: 0 으로 초기화되어 있어야 한다 (malloc 직후 쓰레기값 — ASan 은 0xBE 로 채운다) */
	p = int_array_new(4);
	CHECK(p != NULL);
	if (p) {
		all_zero = 1;
		for (i = 0; i < 4; i++)
			if (p[i] != 0) all_zero = 0;
		CHECK(all_zero);
		for (i = 0; i < 4; i++) p[i] = (int)i + 1; /* 4개 전부 쓸 수 있어야 한다 */
		CHECK(p[3] == 4);

		/* 해제 후 NULL */
		int_array_free(&p);
		CHECK(p == NULL);
		int_array_free(&p); /* 두 번 불러도 double free 가 아니다 */
		CHECK(p == NULL);
	}

	/* 원소 1개 */
	p = int_array_new(1);
	CHECK(p != NULL);
	if (p) {
		CHECK(p[0] == 0);
		p[0] = 42;
		int_array_free(&p);
		CHECK(p == NULL);
	}

	/* 상한(1<<20)까지는 가능, 끝 원소도 0 */
	n = (size_t)1 << 20;
	p = int_array_new(n);
	CHECK(p != NULL);
	if (p) {
		CHECK(p[0] == 0 && p[n - 1] == 0);
		p[n - 1] = 7;
		int_array_free(&p);
	}

	/* 거절해야 하는 입력 */
	CHECK(int_array_new(0) == NULL);                       /* 0 개 */
	CHECK(int_array_new(((size_t)1 << 20) + 1) == NULL);   /* 상한 초과 */
	CHECK(int_array_new(SIZE_MAX / sizeof(int) + 1) == NULL); /* n * sizeof(int) 가 래핑되는 값 */
	CHECK(int_array_new(SIZE_MAX) == NULL);

	/* int_array_free 의 방어 */
	int_array_free(NULL);
	p = NULL;
	int_array_free(&p);
	CHECK(p == NULL);

	return drill_report();
}
