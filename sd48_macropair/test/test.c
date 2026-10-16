#include <stdio.h>
#include "macropair.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_leaves_lock_held(void)
{
    reset_lock();
    do_work_bad();
    T_TRUE(lock_depth() == 1,
        "Bad는 LOCK_END를 짝으로 쓰지 않아 잠금 카운터가 0으로 돌아오지 않아야 한다(취약점 재현)");
}

static void test_bad_accumulates(void)
{
    reset_lock();
    do_work_bad();
    do_work_bad();
    T_TRUE(lock_depth() == 2,
        "Bad를 반복 호출하면 풀리지 않은 잠금이 계속 누적돼야 한다(실전에서는 곧 데드락)");
}

static void test_good_releases_lock(void)
{
    reset_lock();
    do_work_good();
    T_TRUE(lock_depth() == 0, "Good은 짝을 맞춰 호출해 잠금 카운터가 정확히 0으로 돌아와야 한다");
}

int main(void)
{
    test_bad_leaves_lock_held();
    test_bad_accumulates();
    test_good_releases_lock();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
