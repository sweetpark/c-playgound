#include "unbounded.h"

static int g_active = 0;

int active_count(void) { return g_active; }
void reset_active(void) { g_active = 0; }

int handle_connection_bad(void)
{
    g_active++;                        /* 접속마다 상한 없이 생성 */
    return 0;
}

int handle_connection_good(void)
{
    if (g_active >= POOL_MAX) return -1;   /* Pool 초과 시 거부 */
    g_active++;
    return 0;
}
