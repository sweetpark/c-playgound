#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* P036 safe_macros — 모범답안 */

static void *alloc_zeroed_array(size_t n, size_t sz)
{
	if (n == 0 || sz == 0 || n > SIZE_MAX / sz)   /* 곱하기 전에 검사 */
		return NULL;
	return calloc(n, sz);
}

/* do { } while (0) : if (c) FREE(p); else ... 가 깨지지 않는다 */
#define FREE(ptr)                 \
	do {                          \
		free(ptr);                \
		(ptr) = NULL;             \
	} while (0)

/* 식이어야 `if (XNEW(p) == NULL)` 이 가능하다. sizeof(*ptr) 는 ptr 의 타입이 바뀌어도 따라간다 */
#define XNEW(ptr)       ((ptr) = alloc_zeroed_array(1, sizeof(*(ptr))))
#define XNEWN(ptr, n)   ((ptr) = alloc_zeroed_array((n), sizeof(*(ptr))))

static void *(*const keep_alloc_)(size_t, size_t) = alloc_zeroed_array;
