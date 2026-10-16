#include <stdio.h>
#include "padding_check.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    T_TRUE(is_safe_padding_bad(PAD_NONE) == 1,
        "Bad는 패딩 없는(NO_PADDING) RSA도 그대로 허용해야 한다(취약점 재현)");
}

static void test_good_blocks_no_padding(void)
{
    T_TRUE(is_safe_padding_good(PAD_NONE) == 0, "Good은 패딩 없는 RSA를 거부해야 한다");
}

static void test_good_allows_safe_padding(void)
{
    T_TRUE(is_safe_padding_good(PAD_OAEP) == 1, "Good은 OAEP 패딩을 허용해야 한다");
    T_TRUE(is_safe_padding_good(PAD_PKCS1) == 1, "Good은 PKCS1 패딩도 허용해야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_good_blocks_no_padding();
    test_good_allows_safe_padding();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
