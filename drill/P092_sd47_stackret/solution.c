/* P092 stackret — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <string.h>
#include <stdlib.h>
#include "stackret.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wreturn-local-addr"
#pragma GCC diagnostic ignored "-Wdangling-pointer"
char *rpl_bad(void)
{
    char p[10];
    strcpy(p, "SECRET123");
    return p;                    /* 스택 지역 배열의 주소 반환 -- 함수 종료 시 소멸 */
}
#pragma GCC diagnostic pop

char *rpl_good(void)
{
	/* TODO: problem.md 를 보고 Good 구현 */
	return NULL;
}

