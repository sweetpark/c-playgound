#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/wait.h>
#include <unistd.h>
#include "int_alloc.h"

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

static int g_count;
static void call_bad(void) { use_buffer_bad(g_count); }

static void test_bad_is_vulnerable(void)
{
    g_count = INT_MAX - 4;
    T_TRUE(expect_crash(call_bad) == 1,
        "Bad는 오버플로우로 할당이 실패해도 반환값을 안 보고 써서 크래시해야 한다(취약점 재현)");
}

static void test_bad_normal(void)
{
    g_count = 10;
    T_TRUE(expect_crash(call_bad) == 0, "정상 범위의 count는 크래시 없이 동작해야 한다");
}

static void test_good_blocks_overflow(void)
{
    T_TRUE(int_alloc_good(INT_MAX - 4) == NULL,
        "Good은 덧셈 전에 오버플로우를 검사해 NULL을 반환해야 한다");
}

static void test_good_normal(void)
{
    void *buf = use_buffer_good(10);
    T_TRUE(buf != NULL, "정상 범위는 성공해야 한다");
    free(buf);
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_blocks_overflow();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
