/* P093 macropair — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include "macropair.h"

static int g_lock_depth = 0;

#define LOCK_BEGIN() (g_lock_depth++)
#define LOCK_END()   (g_lock_depth--)

int lock_depth(void) { return g_lock_depth; }
void reset_lock(void) { g_lock_depth = 0; }

void do_work_bad(void)
{
    LOCK_BEGIN();
    /* 임계 구역 작업 */
    /* LOCK_END() 짝을 빠뜨림 -> 잠금이 영원히 풀리지 않음 */
}

void do_work_good(void)
{
	/* TODO: problem.md 를 보고 Good 구현 */
	drill_todo_hit = 1; /* 구현하면 이 줄을 지운다 */
}

