#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "stackret.h"

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

static void call_bad(void)
{
    char *p = rpl_bad();
    char c = p[0];             /* 한 바이트라도 직접 역참조해야 ASan이 감지한다 */
    (void)c;
}

static void test_bad_is_vulnerable(void)
{
    T_TRUE(expect_crash(call_bad) == 1,
        "Bad가 반환한 포인터를 역참조하면 크래시해야 한다(취약점 재현)");
}

static void test_good_is_valid_after_return(void)
{
    char *g = rpl_good();
    T_TRUE(g != NULL, "Good은 힙 할당에 성공해야 한다");
    T_TRUE(strcmp(g, "SECRET123") == 0, "Good이 반환한 메모리는 함수 종료 후에도 내용이 그대로 유지돼야 한다");
    free(g);
}

int main(void)
{
    test_bad_is_vulnerable();
    test_good_is_valid_after_return();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
