#include <stdio.h>
#include "mock_login.h"
#include "record_access.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    mock_login_register("sid-alice", "alice");
    T_TRUE(view_record_bad("sid-alice", "bob") == 1,
        "Bad는 alice로 로그인해도 bob의 레코드를 조회할 수 있어야 한다(IDOR 재현)");
}

static void test_bad_normal(void)
{
    T_TRUE(view_record_bad("sid-nobody", "bob") == 0, "로그인 안 된 세션은 거부되어야 한다");
}

static void test_good_blocks_idor(void)
{
    T_TRUE(view_record_good("sid-alice", "bob") == 0,
        "Good은 alice 세션으로 bob 레코드를 조회하면 거부해야 한다");
}

static void test_good_normal(void)
{
    T_TRUE(view_record_good("sid-alice", "alice") == 1,
        "Good은 본인 레코드는 조회를 허용해야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_blocks_idor();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
