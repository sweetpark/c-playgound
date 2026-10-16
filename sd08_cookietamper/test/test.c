#include <stdio.h>
#include "mock_session.h"
#include "admin_check.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    T_TRUE(is_admin_bad("admin") == 1,
        "Bad는 쿠키 값을 'admin' 문자열로 바꾸기만 해도 관리자 취급해야 한다(취약점 재현)");
}

static void test_bad_normal(void)
{
    T_TRUE(is_admin_bad("user") == 0, "일반 role 문자열은 관리자가 아니다");
}

static void test_good_blocks_tampering(void)
{
    T_TRUE(is_admin_good("admin") == 0,
        "Good은 세션 저장소에 없는 값이면 조작해도 관리자가 될 수 없다");
}

static void test_good_normal(void)
{
    mock_session_register("sid-user1", ROLE_USER);
    mock_session_register("sid-admin1", ROLE_ADMIN);

    T_TRUE(is_admin_good("sid-user1") == 0, "일반 사용자 세션은 관리자가 아니다");
    T_TRUE(is_admin_good("sid-admin1") == 1, "서버가 실제로 등록한 관리자 세션만 관리자다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_blocks_tampering();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
