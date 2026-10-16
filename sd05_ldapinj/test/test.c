#include <stdio.h>
#include "ldap_auth.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    T_TRUE(ldap_lookup_bad("*") == 3,
        "Bad는 와일드카드로 디렉터리 전체가 노출되어야 한다(취약점 재현)");
}

static void test_bad_normal(void)
{
    T_TRUE(ldap_lookup_bad("alice") == 1, "정상 조회는 1건");
}

static void test_good_blocks_injection(void)
{
    T_TRUE(ldap_lookup_good("*") == -1, "Good은 와일드카드를 거부해야 한다");
}

static void test_good_normal(void)
{
    T_TRUE(ldap_lookup_good("alice") == 1, "Good도 정상 조회는 통과");
    T_TRUE(ldap_lookup_good("bob") == 1, "다른 정상 사용자도 통과");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_blocks_injection();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
