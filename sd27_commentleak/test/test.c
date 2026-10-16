#include <stdio.h>
#include "release_check.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static const char *g_leaky_source =
    "#include <stdio.h>\n"
    "/* default password is \"abracadabra\" */\n"
    "int verifyAuth(char *ipasswd) { return 0; }\n";

static const char *g_clean_source =
    "#include <stdio.h>\n"
    "/* verifies the user's credentials */\n"
    "int verifyAuth(char *ipasswd) { return 0; }\n";

static void test_bad_is_vulnerable(void)
{
    T_TRUE(release_ok_bad(g_leaky_source) == 1,
        "Bad는 주석에 패스워드가 있어도 배포를 그대로 승인해야 한다(취약점 재현)");
}

static void test_good_blocks_leak(void)
{
    T_TRUE(release_ok_good(g_leaky_source) == 0,
        "Good은 주석에 위험 키워드가 있으면 배포를 막아야 한다");
}

static void test_good_normal(void)
{
    T_TRUE(release_ok_good(g_clean_source) == 1,
        "Good은 위험 키워드가 없는 정상 소스는 배포를 승인해야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_good_blocks_leak();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
