/* P062 intoverflow — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include "int_alloc.h"

#define RESERVE 8

void *int_alloc_bad(int count)
{
    int total = count + RESERVE;
    return malloc((size_t)total * sizeof(int));
}

void use_buffer_bad(int count)
{
    int *buf = (int *)int_alloc_bad(count);
    buf[0] = 42;               /* 반환값 검사 없음 */
    free(buf);
}

void *int_alloc_good(int count)
{
	(void)count;
	/* TODO: problem.md 를 보고 Good 구현 */
	return NULL;
}

void *use_buffer_good(int count)
{
	(void)count;
	/* TODO: problem.md 를 보고 Good 구현 */
	return NULL;
}

