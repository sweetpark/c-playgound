#include <stdio.h>
#include "resleak.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_leaks_on_failure(void)
{
    reset_handle_pool();
    int rc = process_bad(1 /* 늦은 실패 */);
    T_TRUE(rc == -1, "실패 경로이므로 실패를 반환해야 한다");
    T_TRUE(handles_in_use() == 2,
        "Bad는 실패 경로에서 할당했던 핸들 2개를 반환하지 않아 누수돼야 한다(취약점 재현)");
}

static void test_bad_normal_releases(void)
{
    reset_handle_pool();
    int rc = process_bad(0 /* 정상 */);
    T_TRUE(rc == 0, "정상 경로는 성공해야 한다");
    T_TRUE(handles_in_use() == 0, "정상 경로에서는 Bad도 핸들을 모두 반환한다");
}

static void test_good_releases_on_failure(void)
{
    reset_handle_pool();
    int rc = process_good(1 /* 늦은 실패 */);
    T_TRUE(rc == -1, "실패 경로이므로 실패를 반환해야 한다");
    T_TRUE(handles_in_use() == 0,
        "Good은 실패 경로에서도 이미 할당한 핸들을 모두 반환해야 한다(누수 없음)");
}

static void test_good_normal_releases(void)
{
    reset_handle_pool();
    int rc = process_good(0 /* 정상 */);
    T_TRUE(rc == 0, "정상 경로는 성공해야 한다");
    T_TRUE(handles_in_use() == 0, "정상 경로에서도 핸들을 모두 반환해야 한다");
}

int main(void)
{
    test_bad_leaks_on_failure();
    test_bad_normal_releases();
    test_good_releases_on_failure();
    test_good_normal_releases();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
