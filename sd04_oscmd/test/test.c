#include <stdio.h>
#include "cmd_builder.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    T_TRUE(build_and_count_bad("Story.txt; ls") == 2,
        "Bad는 세미콜론 뒤 명령이 추가로 실행되어야 한다(취약점 재현)");
}

static void test_bad_normal(void)
{
    T_TRUE(build_and_count_bad("Story.txt") == 1, "정상 입력은 명령 1개");
}

static void test_good_blocks_injection(void)
{
    T_TRUE(build_and_count_good("Story.txt; ls") == -1, "Good은 위험 문자를 거부해야 한다");
    T_TRUE(build_and_count_good("Story.txt | rm -rf /") == -1, "파이프도 거부");
}

static void test_good_normal(void)
{
    T_TRUE(build_and_count_good("Story.txt") == 1, "Good도 정상 파일명은 그대로 통과(점 포함)");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_blocks_injection();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
