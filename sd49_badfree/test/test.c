#define _DEFAULT_SOURCE
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include "badfree.h"

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

static void test_bad_is_vulnerable(void)
{
    T_TRUE(expect_crash(free_stack_addr_bad) == 1,
        "Bad는 malloc으로 받지 않은 스택 주소를 free해 크래시해야 한다(취약점 재현)");
}

static void test_good_normal(void)
{
    T_TRUE(alloc_and_free_good() == 0, "Good은 malloc으로 받은 주소만 free하므로 정상 종료해야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
