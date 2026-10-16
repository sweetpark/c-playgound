#define _DEFAULT_SOURCE
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include "recur.h"

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

static void call_bad(void) { (void)fac_bad(5); }

static void test_bad_is_vulnerable(void)
{
    T_TRUE(expect_crash(call_bad) == 1,
        "Bad는 종료 조건이 없어 스택이 고갈될 때까지 재귀하다가 크래시해야 한다(취약점 재현)");
}

static void test_good_normal(void)
{
    long out = 0;
    T_TRUE(fac_good(5, &out) == 0, "정상 범위(n=5)는 성공해야 한다");
    T_TRUE(out == 120, "5! = 120 이어야 한다");
}

static void test_good_blocks_deep_recursion(void)
{
    long out = 0;
    T_TRUE(fac_good(100000, &out) == -1,
        "Good은 깊이 제한을 넘으면 크래시 대신 통제된 실패(-1)를 반환해야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_good_normal();
    test_good_blocks_deep_recursion();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
