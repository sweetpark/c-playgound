#include <stdio.h>
#include <unistd.h>
#include "file_perm.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    int mode = create_file_bad("/tmp/sd21_bad.txt");
    T_TRUE(mode == 0666, "Bad는 소유자 외에도 읽고 쓸 수 있는 권한(0666)으로 생성해야 한다(취약점 재현)");
    unlink("/tmp/sd21_bad.txt");
}

static void test_good_blocks_wide_perm(void)
{
    int mode = create_file_good("/tmp/sd21_good.txt");
    T_TRUE(mode == 0600, "Good은 소유자만 읽고 쓸 수 있는 권한(0600)으로 생성해야 한다");
    unlink("/tmp/sd21_good.txt");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_good_blocks_wide_perm();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
