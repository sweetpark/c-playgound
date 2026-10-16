#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "nullderef.h"

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

static void call_bad_missing(void)
{
    char out[64];
    copy_env_bad("MISSING_VAR", out, sizeof(out));
}

static void test_bad_is_vulnerable(void)
{
    T_TRUE(expect_crash(call_bad_missing) == 1,
        "Bad는 알 수 없는 키에서 NULL을 검사 없이 역참조해 크래시해야 한다(취약점 재현)");
}

static void test_bad_normal(void)
{
    char out[64];
    copy_env_bad("HOME", out, sizeof(out));
    T_TRUE(strcmp(out, "/home/appuser") == 0, "알려진 키에서는 크래시 없이 정상 복사돼야 한다");
}

static void test_good_blocks_null(void)
{
    char out[64];
    T_TRUE(copy_env_good("MISSING_VAR", out, sizeof(out)) == -1,
        "Good은 NULL을 검사해 알 수 없는 키에서 실패를 반환해야 한다");
}

static void test_good_normal(void)
{
    char out[64];
    T_TRUE(copy_env_good("HOME", out, sizeof(out)) == 0, "알려진 키에서는 성공해야 한다");
    T_TRUE(strcmp(out, "/home/appuser") == 0, "복사된 내용이 정확해야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_blocks_null();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
