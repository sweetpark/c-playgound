#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

typedef struct {
	long a, b, c;
	char name[40];
} big_t;

int main(void)
{
	char *p;
	big_t *b;
	int *arr;
	int flag = 0;
	size_t i;
	int zero;

	/* FREE: 해제 후 NULL, 두 번 불러도 안전 */
	p = malloc(16);
	CHECK(p != NULL);
	FREE(p);
	CHECK(p == NULL);
	FREE(p);                          /* free(NULL) 이거나 NULL 검사: double free 가 아니다 */
	CHECK(p == NULL);

	/* if/else 사이에서도 한 문장 (중괄호 블록으로 정의하면 else 가 짝을 잃는다) */
	p = malloc(8);
	if (p != NULL)
		FREE(p);
	else
		flag = 1;
	CHECK(flag == 0 && p == NULL);

	/* XNEW: sizeof(*p) 로 크기를 구하고 0 으로 초기화 */
	b = NULL;
	CHECK(XNEW(b) != NULL);
	CHECK(b != NULL);
	if (b) {
		zero = (b->a == 0 && b->b == 0 && b->c == 0 && b->name[0] == '\0' && b->name[39] == '\0');
		CHECK(zero);
		b->c = 5;
		memset(b->name, 'x', sizeof(b->name));   /* 구조체 끝까지 써도 안전해야 한다 (sizeof(b) 였다면 힙 오버플로) */
		FREE(b);
	}
	CHECK(b == NULL);

	/* XNEWN: 원소 n 개, 0 으로 초기화 */
	arr = NULL;
	CHECK(XNEWN(arr, 10) != NULL);
	if (arr) {
		zero = 1;
		for (i = 0; i < 10; i++)
			if (arr[i] != 0) zero = 0;
		CHECK(zero);
		arr[9] = 1;
		FREE(arr);
	}

	/* n == 0 이거나 n * sizeof 가 넘치면 NULL (arr 도 NULL) */
	arr = (int *)0x1;
	CHECK(XNEWN(arr, 0) == NULL);
	CHECK(arr == NULL);
	arr = (int *)0x1;
	CHECK(XNEWN(arr, SIZE_MAX / sizeof(int) + 1) == NULL);
	CHECK(arr == NULL);

	return drill_report();
}
