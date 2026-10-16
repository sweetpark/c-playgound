#include <stdio.h>
#include <string.h>
#include "db_login.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    char out[128];
    db_login_bad("x", out, sizeof(out));
    T_TRUE(strstr(out, "user=root") != NULL,
        "Bad는 항상 계정명이 root로 고정되어야 한다(취약점 재현)");
}

static void test_bad_cannot_change_account(void)
{
    char out1[128], out2[128];
    db_login_bad("pw1", out1, sizeof(out1));
    db_login_bad("pw2", out2, sizeof(out2));
    T_TRUE(strstr(out1, "user=root") != NULL && strstr(out2, "user=root") != NULL,
        "Bad는 패스워드가 바뀌어도 계정명은 절대 못 바꾼다");
}

static void test_good_uses_provided_account(void)
{
    char out[128];
    db_login_good("svc_alice", "x", out, sizeof(out));
    T_TRUE(strstr(out, "user=svc_alice") != NULL, "Good은 전달받은 계정명을 그대로 사용해야 한다");
}

static void test_good_changes_with_input(void)
{
    char out1[128], out2[128];
    db_login_good("acct-one", "x", out1, sizeof(out1));
    db_login_good("acct-two", "x", out2, sizeof(out2));
    T_TRUE(strcmp(out1, out2) != 0, "Good은 계정명을 바꿔 부르면 결과도 달라져야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_cannot_change_account();
    test_good_uses_provided_account();
    test_good_changes_with_input();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
