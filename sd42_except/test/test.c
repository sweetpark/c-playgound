#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "except.h"

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

static void call_bad_fail(void)
{
    char fromBuf[10], toBuf[10];
    copy_input_bad(fromBuf, sizeof(fromBuf), 1 /* 읽기 실패 */, toBuf, sizeof(toBuf));
}

static void test_bad_is_vulnerable(void)
{
    T_TRUE(expect_crash(call_bad_fail) == 1,
        "Bad는 읽기 실패로 널 종료가 안 된 fromBuf를 그대로 strcpy해 오버플로우로 크래시해야 한다(취약점 재현)");
}

static void test_bad_normal(void)
{
    char fromBuf[10], toBuf[10];
    copy_input_bad(fromBuf, sizeof(fromBuf), 0 /* 정상 읽기 */, toBuf, sizeof(toBuf));
    T_TRUE(strcmp(toBuf, "hi") == 0, "정상 읽기에서는 크래시 없이 내용이 그대로 복사돼야 한다");
}

static void test_good_blocks_on_failure(void)
{
    char fromBuf[10];
    char toBuf[10] = "SENTINEL";
    int rc = copy_input_good(fromBuf, sizeof(fromBuf), 1 /* 읽기 실패 */, toBuf, sizeof(toBuf));
    T_TRUE(rc == -1, "Good은 읽기 실패를 감지하면 복사를 하지 않고 실패를 반환해야 한다");
}

static void test_good_normal(void)
{
    char fromBuf[10], toBuf[10];
    int rc = copy_input_good(fromBuf, sizeof(fromBuf), 0 /* 정상 읽기 */, toBuf, sizeof(toBuf));
    T_TRUE(rc == 0, "정상 읽기는 성공해야 한다");
    T_TRUE(strcmp(toBuf, "hi") == 0, "정상 읽기에서는 내용이 정확히 복사돼야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_blocks_on_failure();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
