#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

static int data_is(const blob_t *b, const int *want, size_t n)
{
	size_t i;
	if (b == NULL || b->n != n) return 0;
	for (i = 0; i < n; i++)
		if (b->data[i] != want[i]) return 0;
	return 1;
}

int main(void)
{
	blob_t *a, *c;
	size_t i;
	int zero;
	int want[4] = {1, 11, 21, 31};
	int one[1] = {5};

	/* blob_new: 0 으로 초기화 */
	a = blob_new(4);
	CHECK(a != NULL);
	if (a) {
		CHECK(a->n == 4);
		zero = 1;
		for (i = 0; i < 4; i++)
			if (a->data[i] != 0) zero = 0;
		CHECK(zero);
		for (i = 0; i < 4; i++) a->data[i] = (int)(i * 10 + 1);
		CHECK(blob_sum(a) == 64);

		/* blob_clone: 헤더만이 아니라 data 까지 깊은 복사 */
		c = blob_clone(a);
		CHECK(c != NULL);
		CHECK(c != a);
		CHECK(data_is(c, want, 4));
		if (c) {
			a->data[0] = 999;           /* 원본을 바꿔도 복사본은 그대로 */
			CHECK(c->data[0] == 1);
			CHECK(blob_sum(c) == 64);
			free(c);
		}
		free(a);
	}

	/* 원소 1개 */
	a = blob_new(1);
	CHECK(a != NULL);
	if (a) {
		a->data[0] = 5;
		c = blob_clone(a);
		CHECK(data_is(c, one, 1));
		free(c);
		free(a);
	}

	/* 상한 1024 */
	a = blob_new(1024);
	CHECK(a != NULL);
	if (a) {
		a->data[1023] = 7;
		c = blob_clone(a);
		CHECK(c != NULL && c->n == 1024 && c->data[1023] == 7 && c->data[0] == 0);
		free(c);
		free(a);
	}

	/* 거절 */
	CHECK(blob_new(0) == NULL);
	CHECK(blob_new(1025) == NULL);
	CHECK(blob_clone(NULL) == NULL);
	CHECK(blob_sum(NULL) == 0);

	return drill_report();
}
