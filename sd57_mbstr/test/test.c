#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "mbstr.h"

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
    char dst[8];
    mb_copy_bad(dst, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
}

static void test_bad_is_vulnerable(void)
{
    T_TRUE(expect_crash(call_bad) == 1,
        "Bad는 크기 인자가 없어 대상 버퍼보다 긴 입력에서 크래시해야 한다(취약점 재현)");
}

static void test_good_respects_utf8_boundary(void)
{
    char dst[5];   /* "caf" + '\0' 까지만 들어가는 크기 -- é(0xC3 0xA9)는 중간에 걸림 */
    T_TRUE(mb_copy_good(dst, sizeof(dst), "caf\xC3\xA9") == 0, "Good은 항상 성공해야 한다(잘라서라도 완료)");
    T_TRUE(strcmp(dst, "caf") == 0,
        "Good은 멀티바이트 문자 중간에서 자르지 않고 문자 경계까지 물러나야 한다(잘린 é 바이트가 남으면 안 됨)");
}

static void test_good_normal(void)
{
    char dst[16];
    T_TRUE(mb_copy_good(dst, sizeof(dst), "hi") == 0, "정상 크기 입력은 성공해야 한다");
    T_TRUE(strcmp(dst, "hi") == 0, "짧은 입력은 그대로 복사돼야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_good_respects_utf8_boundary();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
