#include <stdio.h>
#include "port_guard.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_accepts_dangerous_port(void)
{
    T_TRUE(parse_port_bad("22") == 22, "Bad는 SSH 포트도 그대로 받아들여야 한다(취약점 재현)");
    T_TRUE(parse_port_bad("99999") == 99999, "Bad는 유효 범위 밖 포트도 그대로 반환");
}

static void test_bad_normal(void)
{
    T_TRUE(parse_port_bad("3002") == 3002, "정상 포트는 그대로 반환");
}

static void test_good_rejects_out_of_range(void)
{
    T_TRUE(parse_port_good("22") == 3000, "Good은 범위 밖이면 기본값(3000)으로 대체");
    T_TRUE(parse_port_good("99999") == 3000, "Good은 65535 초과도 기본값으로 대체");
}

static void test_good_allows_normal(void)
{
    T_TRUE(parse_port_good("3002") == 3002, "Good도 허용 범위 안 값은 그대로 통과");
}

int main(void)
{
    test_bad_accepts_dangerous_port();
    test_bad_normal();
    test_good_rejects_out_of_range();
    test_good_allows_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
