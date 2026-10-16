#include <stdio.h>
#include "key_length.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    T_TRUE(validate_key_length_bad(56) == 1,
        "Bad는 DES 수준의 56비트 키도 그대로 통과시켜야 한다(취약점 재현)");
    T_TRUE(validate_key_length_bad(0) == 1, "Bad는 0비트도 통과시킨다");
}

static void test_good_blocks_short_key(void)
{
    T_TRUE(validate_key_length_good(56) == 0, "Good은 56비트 키를 거부해야 한다");
    T_TRUE(validate_key_length_good(64) == 0, "Good은 64비트 키도 거부해야 한다");
}

static void test_good_normal(void)
{
    T_TRUE(validate_key_length_good(128) == 1, "Good은 128비트 키는 통과시켜야 한다(경계값)");
    T_TRUE(validate_key_length_good(256) == 1, "Good은 256비트 키도 통과시켜야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_good_blocks_short_key();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
