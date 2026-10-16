#include <stdio.h>
#include "cipher_check.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    T_TRUE(is_allowed_cipher_bad("DES-ECB") == 1,
        "Bad는 DES-ECB 같은 취약 알고리즘도 그대로 허용해야 한다(취약점 재현)");
    T_TRUE(is_allowed_cipher_bad("MD5") == 1, "Bad는 MD5도 허용해야 한다");
}

static void test_good_blocks_weak(void)
{
    T_TRUE(is_allowed_cipher_good("DES-ECB") == 0, "Good은 DES-ECB를 거부해야 한다");
    T_TRUE(is_allowed_cipher_good("RC4") == 0, "Good은 RC4를 거부해야 한다");
    T_TRUE(is_allowed_cipher_good("SHA1") == 0, "Good은 SHA1을 거부해야 한다");
}

static void test_good_allows_strong(void)
{
    T_TRUE(is_allowed_cipher_good("AES-256-GCM") == 1, "Good은 AES-256-GCM을 허용해야 한다");
}

static void test_good_default_deny(void)
{
    T_TRUE(is_allowed_cipher_good("SOME-UNKNOWN-CIPHER") == 0,
        "Good은 목록에 없는 미지의 알고리즘도 화이트리스트 원칙상 거부해야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_good_blocks_weak();
    test_good_allows_strong();
    test_good_default_deny();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
