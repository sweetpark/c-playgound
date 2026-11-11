/* P083 recursion — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include "recur.h"

#define MAX_DEPTH 10000

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Winfinite-recursion"
long fac_bad(int n)
{
    return n * fac_bad(n - 1);      /* 종료 조건이 없어 무한 재귀 -> 스택 고갈 */
}
#pragma GCC diagnostic pop

int fac_good(int n, long *out)
{
	(void)n;
	(void)out;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

