#include <stdio.h>
#include "threadexit.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static int g_result_bad;
static int g_result_good;

static void test_bad_leaves_thread_unreaped(void)
{
    reset_unreaped();
    T_TRUE(spawn_worker_bad(&g_result_bad) == 0, "스레드 생성 자체는 성공해야 한다");
    T_TRUE(unreaped_count() == 1,
        "Bad는 join도 detach도 하지 않아 생성한 스레드가 회수되지 않은 채로 남아야 한다(취약점 재현)");
}

static void test_good_reaps_thread(void)
{
    reset_unreaped();
    T_TRUE(spawn_worker_good(&g_result_good) == 0, "스레드 생성과 join이 모두 성공해야 한다");
    T_TRUE(unreaped_count() == 0, "Good은 join으로 즉시 회수해 미회수 카운트가 0이어야 한다");
    T_TRUE(g_result_good == 42, "join으로 완료를 기다렸으므로 워커의 결과가 반영돼 있어야 한다");
}

int main(void)
{
    test_bad_leaves_thread_unreaped();
    test_good_reaps_thread();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
