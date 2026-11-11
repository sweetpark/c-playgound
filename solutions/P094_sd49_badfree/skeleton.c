/* P094 badfree — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <stdlib.h>
#include "badfree.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"
void free_stack_addr_bad(void)
{
    char p[10];
    free(p);                          /* 스택 주소 해제 -> 비정상 동작 */
}
#pragma GCC diagnostic pop

int alloc_and_free_good(void)
{
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

