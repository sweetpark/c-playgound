#include <stdio.h>
#include "priv_ops.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    reset_priv();
    elevate_bad();
    T_TRUE(get_current_priv() == 0,
        "Bad는 권한 상승 후 계속 root(0) 상태로 남아있어야 한다(취약점 재현)");
}

static void test_good_restores_priv(void)
{
    reset_priv();
    int before = get_current_priv();
    do_privileged_work_good();
    T_TRUE(get_current_priv() == before,
        "Good은 작업이 끝나면 상승 전 권한으로 정확히 복귀해야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_good_restores_priv();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
