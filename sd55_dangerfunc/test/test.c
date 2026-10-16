#include <stdio.h>
#include "dangerfunc.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_vfork_corrupts_parent(void)
{
    int r = demonstrate_vfork_bad();
    T_TRUE(r == 999,
        "vfork는 자식이 부모와 주소 공간을 공유해 부모의 스택 변수까지 바꿔버려야 한다(취약점 재현)");
}

static void test_good_fork_isolates_parent(void)
{
    int r = demonstrate_fork_good();
    T_TRUE(r == 100,
        "fork는 자식이 독립된 복사본에서 동작해 부모 값이 그대로 보존돼야 한다");
}

int main(void)
{
    test_bad_vfork_corrupts_parent();
    test_good_fork_isolates_parent();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
