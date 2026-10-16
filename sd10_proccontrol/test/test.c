#include <stdio.h>
#include <string.h>
#include "doc_convert.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    char buf[128];
    T_TRUE(build_convert_command_bad("rm -rf /tmp/x; echo pwned", "report.doc",
        buf, sizeof(buf)) == 0, "Bad는 조립 자체는 항상 성공해야 한다");
    T_TRUE(strstr(buf, "pwned") != NULL,
        "Bad는 op 문자열이 그대로 명령어가 되어 임의 명령이 섞여 들어가야 한다(취약점 재현)");
}

static void test_bad_normal(void)
{
    char buf[128];
    build_convert_command_bad("DocToHtml", "report.doc", buf, sizeof(buf));
    T_TRUE(strcmp(buf, "DocToHtml report.doc") == 0, "정상 op도 그대로 조립된다");
}

static void test_good_blocks_arbitrary_op(void)
{
    char buf[128];
    T_TRUE(build_convert_command_good("rm -rf /tmp/x; echo pwned", "report.doc",
        buf, sizeof(buf)) == -1, "Good은 매핑에 없는 op를 거부해야 한다");
}

static void test_good_normal(void)
{
    char buf[128];
    T_TRUE(build_convert_command_good("DocToHtml", "report.doc", buf, sizeof(buf)) == 0,
        "매핑에 있는 op는 통과해야 한다");
    T_TRUE(strcmp(buf, "convertDocToHtml report.doc") == 0,
        "실행되는 명령은 사용자 입력이 아니라 매핑된 고정 문자열이어야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_blocks_arbitrary_op();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
