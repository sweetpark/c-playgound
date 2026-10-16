#include <stdio.h>
#include "erract.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_hides_failure(void)
{
    int balance = 500;
    int rc = process_request_bad(1 /* step1 실패 */, &balance);
    T_TRUE(rc == 0, "Bad는 내부 단계가 실패해도 항상 성공(0)을 보고해야 한다(실패 은폐 재현)");
    T_TRUE(balance == 400,
        "Bad는 실패를 감지하고도 아무 조치가 없어 잔액 차감이 그대로 진행돼야 한다(상태 오염 재현)");
}

static void test_good_reports_failure(void)
{
    int balance = 500;
    int rc = process_request_good(1 /* step1 실패 */, &balance);
    T_TRUE(rc == -1, "Good은 내부 단계가 실패하면 그 실패를 그대로 호출자에게 알려야 한다");
    T_TRUE(balance == 500, "Good은 실패 시 즉시 중단해 잔액이 전혀 바뀌지 않아야 한다");
}

static void test_good_normal_path(void)
{
    int balance = 500;
    int rc = process_request_good(0 /* step1 성공 */, &balance);
    T_TRUE(rc == 0, "정상 경로는 성공을 보고해야 한다");
    T_TRUE(balance == 400, "정상 경로는 잔액이 정확히 차감돼야 한다");
}

int main(void)
{
    test_bad_hides_failure();
    test_good_reports_failure();
    test_good_normal_path();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
