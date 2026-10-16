#include <stdio.h>
#include "service_port.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    T_TRUE(resolve_service_port_bad("22") == 22,
        "Bad는 외부 입력을 그대로 서비스 포트로 써서 임의 포트를 지정할 수 있어야 한다(취약점 재현)");
    T_TRUE(resolve_service_port_bad("0") == 0, "0번 포트 같은 비정상 값도 그대로 통과");
}

static void test_bad_normal(void)
{
    T_TRUE(resolve_service_port_bad("21") == 21, "정상 포트 문자열도 그대로 반환");
}

static void test_good_blocks_arbitrary_value(void)
{
    T_TRUE(resolve_service_port_good(99) == -1, "Good은 범위 밖 인덱스를 거부해야 한다");
    T_TRUE(resolve_service_port_good(-5) == -1, "음수 인덱스도 거부");
}

static void test_good_normal(void)
{
    T_TRUE(resolve_service_port_good(0) == 21, "인덱스 0은 기본 포트(21)");
    T_TRUE(resolve_service_port_good(1) == 2121, "인덱스 1은 대체 포트(2121)");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_blocks_arbitrary_value();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
