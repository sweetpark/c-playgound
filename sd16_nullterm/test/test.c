#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "raw_copy.h"

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

static char *g_raw;
static char g_out[256];
static void call_bad(void) { copy_raw_bad(g_raw, g_out); }

static void test_bad_is_vulnerable(void)
{
    g_raw = malloc(8);
    memset(g_raw, 'A', 8);          /* 정확히 8바이트, 널 종료 없음 */
    T_TRUE(expect_crash(call_bad) == 1,
        "Bad는 널 종료 안 된 버퍼를 strcpy로 읽으면 경계를 넘어 크래시해야 한다(취약점 재현)");
    free(g_raw);
}

static void test_bad_normal(void)
{
    g_raw = malloc(8);
    memset(g_raw, 0, 8);
    memcpy(g_raw, "hi", 2);
    T_TRUE(expect_crash(call_bad) == 0, "정상 널 종료 문자열은 크래시 없이 동작해야 한다");
    free(g_raw);
}

static void test_good_blocks_overread(void)
{
    char raw[8];
    char out[4];
    memset(raw, 'A', 8);
    T_TRUE(copy_raw_good(raw, 8, out, sizeof(out)) == -1,
        "Good은 out 버퍼가 raw_len을 다 담을 수 없으면 실패를 알려야 한다");
    T_TRUE(out[3] == '\0', "그래도 out은 항상 널로 끝나야 한다");
}

static void test_good_normal(void)
{
    char raw[8];
    char out[16];
    memset(raw, 0, 8);
    memcpy(raw, "hi", 2);
    T_TRUE(copy_raw_good(raw, 2, out, sizeof(out)) == 0, "정상 길이는 성공해야 한다");
    T_TRUE(strcmp(out, "hi") == 0, "복사된 내용이 정확해야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_blocks_overread();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
