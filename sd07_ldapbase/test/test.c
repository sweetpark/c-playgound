#include <stdio.h>
#include "ldap_base.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_normal(void)
{
    T_TRUE(ldap_by_manager_bad("ou=NewHires") == 2,
        "정상 base면 NewHires 부서 2명(alice,bob)만 매칭");
}

static void test_bad_is_vulnerable(void)
{
    T_TRUE(ldap_by_manager_bad("*") == 3,
        "Bad는 base가 오염되면 스코프가 무력화되어 다른 부서(carol)까지 노출된다(취약점 재현)");
}

static void test_good_always_scoped(void)
{
    T_TRUE(ldap_by_manager_good() == 2,
        "Good은 base를 상수로 고정했으므로 항상 NewHires 2명만 나온다(우회 자체가 불가능)");
}

int main(void)
{
    test_bad_normal();
    test_bad_is_vulnerable();
    test_good_always_scoped();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
