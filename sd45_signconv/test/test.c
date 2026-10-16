#include <stdio.h>
#include <limits.h>
#include "signconv.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_wraps_to_uint_max(void)
{
    T_TRUE(len_bad(NULL) == UINT_MAX,
        "Bad는 NULL에서 -1을 unsigned로 반환해 UINT_MAX로 감겨야 한다(취약점 재현)");
}

static void test_bad_normal(void)
{
    T_TRUE(len_bad("hi") == 2, "정상 문자열은 길이를 정확히 반환해야 한다");
}

static void test_good_returns_zero(void)
{
    T_TRUE(len_good(NULL) == 0, "Good은 NULL에서 무부호에 안전한 0을 반환해야 한다");
}

static void test_good_normal(void)
{
    T_TRUE(len_good("hi") == 2, "정상 문자열은 길이를 정확히 반환해야 한다");
}

static void test_bad_bounds_check_bypassed(void)
{
    T_TRUE(within_len_bad(999999u, NULL) != 0,
        "Bad는 -1이 UINT_MAX로 감겨서 어떤 큰 idx도 범위 안으로 착각해야 한다(경계검사 무력화 재현)");
}

static void test_good_bounds_check_holds(void)
{
    T_TRUE(within_len_good(999999u, NULL) == 0,
        "Good은 0을 반환하므로 같은 idx가 범위 밖으로 올바르게 거부돼야 한다");
}

int main(void)
{
    test_bad_wraps_to_uint_max();
    test_bad_normal();
    test_good_returns_zero();
    test_good_normal();
    test_bad_bounds_check_bypassed();
    test_good_bounds_check_holds();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
