#include <stdio.h>
#include <string.h>
#include "db_conn.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    char out[128];
    db_connect_bad("db1", "svc", out, sizeof(out));
    T_TRUE(strstr(out, "pw=asdf") != NULL,
        "Bad는 어떤 호출이든 하드코딩된 패스워드(asdf)를 그대로 써야 한다(취약점 재현)");
}

static void test_bad_cannot_be_changed(void)
{
    char out1[128], out2[128];
    db_connect_bad("db1", "svc", out1, sizeof(out1));
    db_connect_bad("db2", "svc", out2, sizeof(out2));
    T_TRUE(strstr(out1, "pw=asdf") != NULL && strstr(out2, "pw=asdf") != NULL,
        "Bad는 서버가 바뀌어도 패스워드를 절대 바꿀 수 없다(고칠 수 없는 백도어)");
}

static void test_good_uses_provided_password(void)
{
    char out[128];
    db_connect_good("db1", "svc", "s3cr3t!", out, sizeof(out));
    T_TRUE(strstr(out, "pw=s3cr3t!") != NULL, "Good은 전달받은 패스워드를 그대로 사용해야 한다");
}

static void test_good_changes_with_input(void)
{
    char out1[128], out2[128];
    db_connect_good("db1", "svc", "pw-one", out1, sizeof(out1));
    db_connect_good("db1", "svc", "pw-two", out2, sizeof(out2));
    T_TRUE(strcmp(out1, out2) != 0,
        "Good은 패스워드를 바꿔 부르면 결과도 달라져야 한다(교체 가능함을 증명)");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_cannot_be_changed();
    test_good_uses_provided_password();
    test_good_changes_with_input();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
