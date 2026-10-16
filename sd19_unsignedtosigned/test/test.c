#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "chunk_copy.h"

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

static char g_dst[16], g_src[16];
static void call_bad(void) { chunk_copy_bad(g_dst, g_src, 0, 8); }

static void test_bad_is_vulnerable(void)
{
    T_TRUE(expect_crash(call_bad) == 1,
        "Bad는 초기화 실패(-1)가 거대한 unsigned 크기로 바뀌어 memcpy가 폭주해야 한다(취약점 재현)");
}

static void test_bad_normal(void)
{
    char dst[16], src[16] = "hello";
    chunk_copy_bad(dst, src, 1, 6);
    T_TRUE(strcmp(dst, "hello") == 0, "정상 초기화 상태는 크래시 없이 동작해야 한다");
}

static void test_good_blocks_wraparound(void)
{
    char dst[16], src[16] = {0};
    T_TRUE(chunk_copy_good(dst, sizeof(dst), src, 0, 8) == -1,
        "Good은 초기화 실패(-1)를 부호 있는 채로 확인해 거부해야 한다");
}

static void test_good_normal(void)
{
    char dst[16] = {0}, src[16] = "hello";
    T_TRUE(chunk_copy_good(dst, sizeof(dst), src, 1, 6) == 0, "정상 초기화 상태는 성공해야 한다");
    T_TRUE(strcmp(dst, "hello") == 0, "복사된 내용이 정확해야 한다");
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
