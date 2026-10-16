#include <stdio.h>
#include "uid_resolve.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    T_TRUE(resolve_uid_bad(1) == 0,
        "Bad는 정상 로그인만 하면 손님 계정도 root(0)를 받아야 한다(취약점 재현)");
}

static void test_bad_normal(void)
{
    T_TRUE(resolve_uid_bad(0) == -1, "인증 실패는 권한이 없어야 한다");
}

static void test_good_blocks_over_grant(void)
{
    T_TRUE(resolve_uid_good(GRP_GUEST) != 0, "Good은 게스트 그룹에 root(0)를 주면 안 된다");
    T_TRUE(resolve_uid_good(GRP_UNKNOWN) == -1, "Good은 모르는 그룹은 권한 없음으로 처리해야 한다");
}

static void test_good_normal(void)
{
    T_TRUE(resolve_uid_good(GRP_ADMIN) == 0, "관리자 그룹만 root(0)를 받아야 한다");
    T_TRUE(resolve_uid_good(GRP_NORMAL) == 1000, "일반 그룹은 일반 UID를 받아야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_blocks_over_grant();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
