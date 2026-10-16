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
    LOCK_BEGIN();
    /* 임계 구역 작업 */
    LOCK_END();
}
