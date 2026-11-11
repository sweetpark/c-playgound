#include "priv_ops.h"

#define NORMAL_UID 1000
#define ROOT_UID   0

static int g_current_priv = NORMAL_UID;

int get_current_priv(void) { return g_current_priv; }
void reset_priv(void) { g_current_priv = NORMAL_UID; }

void elevate_bad(void)
{
    g_current_priv = ROOT_UID;
}

void do_privileged_work_good(void)
{
    int backup = g_current_priv;
    g_current_priv = ROOT_UID;
    /* ... 필요한 작업만 이 구간에서 수행한다고 가정 ... */
    g_current_priv = backup;
}
