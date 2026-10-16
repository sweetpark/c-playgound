#include <stdio.h>
#include "unbounded.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

#define ATTEMPTS 10

static void test_bad_is_unbounded(void)
{
    int i;
    reset_active();
    for (i = 0; i < ATTEMPTS; i++) {
        handle_connection_bad();
    }
    T_TRUE(active_count() == ATTEMPTS,
        "Bad는 상한이 없어 요청한 만큼(10개) 전부 활성 연결로 쌓여야 한다(취약점 재현)");
}

static void test_good_caps_at_pool_max(void)
{
    int i, accepted = 0, rejected = 0;
    reset_active();
    for (i = 0; i < ATTEMPTS; i++) {
        if (handle_connection_good() == 0) accepted++;
        else rejected++;
    }
    T_TRUE(accepted == POOL_MAX, "Good은 POOL_MAX(4)개까지만 수락해야 한다");
    T_TRUE(rejected == ATTEMPTS - POOL_MAX, "나머지 요청은 거부돼야 한다");
    T_TRUE(active_count() == POOL_MAX, "활성 연결 수는 POOL_MAX를 절대 넘지 않아야 한다");
}

int main(void)
{
    test_bad_is_unbounded();
    test_good_caps_at_pool_max();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
