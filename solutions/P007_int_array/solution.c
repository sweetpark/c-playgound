#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* P007 int_array — 모범답안 */
#define INT_ARRAY_MAX ((size_t)1 << 20)

int *int_array_new(size_t n)
{
	if (n == 0 || n > INT_ARRAY_MAX)
		return NULL;
	if (n > SIZE_MAX / sizeof(int))     /* 곱셈 오버플로 (32비트 size_t 에서 의미) */
		return NULL;
	return calloc(n, sizeof(int));      /* 0 으로 초기화 */
}

void int_array_free(int **pp)
{
	if (pp == NULL)
		return;
	free(*pp);                          /* free(NULL) 은 안전 */
	*pp = NULL;
}
