#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "heap_ops.h"

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

static const char *g_input;
static void call_bad(void) { heap_copy_bad(g_input); }

static void test_bad_is_vulnerable(void)
{
    g_input = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    T_TRUE(expect_crash(call_bad) == 1,
        "Bad는 10바이트를 넘는 입력에서 힙 오버플로우로 크래시해야 한다(취약점 재현)");
}

static void test_bad_normal(void)
{
    g_input = "short";
    T_TRUE(expect_crash(call_bad) == 0, "짧은 입력은 크래시 없이 정상 동작해야 한다");
}

static void test_good_blocks_overflow(void)
{
    char *out = NULL;
    T_TRUE(heap_copy_good("AAAAAAAAAAAAAAAAAAAA", &out) == -1,
        "Good은 버퍼보다 긴 입력을 거부해야 한다");
}

static void test_good_normal(void)
{
    char *out = NULL;
    T_TRUE(heap_copy_good("short", &out) == 0, "정상 입력은 성공해야 한다");
    T_TRUE(out != NULL && strcmp(out, "short") == 0, "복사된 내용이 정확해야 한다");
    free(out);
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
