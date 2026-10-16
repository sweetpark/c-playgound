#include <stdio.h>
#include <string.h>
#include "errmsg.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static const char *g_detail = "/home/appuser/.secret_config (MYPATH=/home/appuser/.secret_config)";

static void test_bad_leaks_detail(void)
{
    char user_msg[256];
    report_error_bad(g_detail, user_msg, sizeof(user_msg));
    T_TRUE(strstr(user_msg, g_detail) != NULL,
        "Bad는 상세 경로를 사용자 메시지에 그대로 노출해야 한다(취약점 재현)");
}

static void test_good_hides_detail_from_user(void)
{
    char user_msg[256], log_msg[256];
    report_error_good(g_detail, user_msg, sizeof(user_msg), log_msg, sizeof(log_msg));
    T_TRUE(strstr(user_msg, g_detail) == NULL,
        "Good은 사용자 메시지에 상세 경로가 절대 섞이면 안 된다");
}

static void test_good_keeps_detail_in_log(void)
{
    char user_msg[256], log_msg[256];
    report_error_good(g_detail, user_msg, sizeof(user_msg), log_msg, sizeof(log_msg));
    T_TRUE(strstr(log_msg, g_detail) != NULL,
        "Good은 로그에는 상세 내용을 그대로 남겨야 한다(운영 디버깅을 위해)");
}

int main(void)
{
    test_bad_leaks_detail();
    test_good_hides_detail_from_user();
    test_good_keeps_detail_in_log();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
