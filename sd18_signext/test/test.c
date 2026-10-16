#define _DEFAULT_SOURCE
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include "info_lookup.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static int expect_crash(void (*fn)(void))
{
    pid_t pid = fork();
    if (pid == 0) { fn(); _exit(0); }
    int status = 0;
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status)) return 1;
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) return 1;
    return 0;
}

static int g_id;
static int g_info[300];
static void call_bad(void) { (void)lookup_info_bad(g_id, g_info); }

static void test_bad_is_vulnerable(void)
{
    g_id = 40000;
    T_TRUE(expect_crash(call_bad) == 1,
        "Bad는 short로 잘리며 음수가 된 값이 거대한 unsigned 인덱스가 되어 크래시해야 한다(취약점 재현)");
}

static void test_bad_normal(void)
{
    g_id = 10;
    T_TRUE(expect_crash(call_bad) == 0, "정상 범위의 id는 크래시 없이 동작해야 한다");
}

static void test_good_blocks_wraparound(void)
{
    T_TRUE(lookup_info_good(40000, g_info, 300) == -1,
        "Good은 범위를 넘는 id를 원본 int로 직접 검사해 거부해야 한다");
    T_TRUE(lookup_info_good(-1, g_info, 300) == -1, "음수 id도 거부해야 한다");
}

static void test_good_normal(void)
{
    g_info[10] = 99;
    T_TRUE(lookup_info_good(10, g_info, 300) == 99, "정상 id는 값을 정확히 반환해야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_blocks_wraparound();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
