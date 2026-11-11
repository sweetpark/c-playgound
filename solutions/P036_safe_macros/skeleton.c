#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* P036 safe_macros — 요구사항은 problem.md */

/* n 개 x sz 바이트를 0 으로 초기화해 할당. n == 0 이거나 n * sz 가 넘치면 NULL */
static void *alloc_zeroed_array(size_t n, size_t sz)
{
	(void)n;
	(void)sz;
	/* TODO */
	return NULL;
}

/* ptr 을 해제하고 NULL 로. NULL 이어도 안전. if/else 사이에서 한 문장으로 */
#define FREE(ptr) ((void)0) /* TODO */

/* ptr = (sizeof(*ptr) 만큼 0 초기화된 새 객체).  식의 값은 그 포인터 */
#define XNEW(ptr) ((ptr) = NULL) /* TODO */

/* ptr = (원소 n 개 0 초기화된 배열).  식의 값은 그 포인터 */
#define XNEWN(ptr, n) ((ptr) = NULL) /* TODO */

/* 컴파일러 경고 방지용 (지우지 마세요) */
static void *(*const keep_alloc_)(size_t, size_t) = alloc_zeroed_array;
