#define _DEFAULT_SOURCE
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include "array_ops.h"

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
static void call_bad(void) { write_before_start_bad(g_count); }

static void test_bad_is_vulnerable(void)
{
    g_count = 11;
    T_TRUE(expect_crash(call_bad) == 1,
        "Bad는 토큰이 배열 크기보다 많으면 버퍼 시작 이전에 써서 크래시해야 한다(취약점 재현)");
}

static void test_bad_normal(void)
{
    g_count = 10;
    T_TRUE(expect_crash(call_bad) == 0, "배열 크기와 같은 개수는 크래시 없이 정상 동작");
}

static void test_good_blocks_underflow(void)
{
    int arr[10];
    T_TRUE(write_before_start_good(11, arr, 10) == -1,
        "Good은 배열 크기를 넘는 개수를 거부해야 한다");
}

static void test_good_normal(void)
{
    int arr[10] = {0};
    T_TRUE(write_before_start_good(10, arr, 10) == 0, "정상 개수는 성공해야 한다");
    T_TRUE(arr[0] == 9, "역순으로 채워진 마지막 값이 정확해야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_blocks_underflow();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
