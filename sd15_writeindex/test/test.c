#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "arr_write.h"

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

static int *g_arr;
static int g_num;
static void call_bad(void) { write_at_index_bad(g_arr, g_num, 99); }

static void test_bad_is_vulnerable(void)
{
    int *arr = malloc(5 * sizeof(int));
    g_arr = arr; g_num = 10;
    T_TRUE(expect_crash(call_bad) == 1,
        "Bad는 배열 크기를 넘는 인덱스에 쓰면 크래시해야 한다(취약점 재현)");
    free(arr);
}

static void test_bad_normal(void)
{
    int *arr = malloc(5 * sizeof(int));
    g_arr = arr; g_num = 5;
    T_TRUE(expect_crash(call_bad) == 0, "유효 인덱스는 크래시 없이 정상 동작해야 한다");
    free(arr);
}

static void test_good_blocks_overwrite(void)
{
    int arr[5] = {0};
    T_TRUE(write_at_index_good(arr, 5, 10, 99) == -1, "Good은 상한 밖 인덱스를 거부해야 한다");
    T_TRUE(write_at_index_good(arr, 5, 0, 99) == -1, "Good은 0(하한 밖)도 거부해야 한다");
}

static void test_good_normal(void)
{
    int arr[5] = {0};
    T_TRUE(write_at_index_good(arr, 5, 5, 99) == 0, "정상 인덱스는 성공해야 한다");
    T_TRUE(arr[4] == 99, "쓰인 값이 정확해야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_blocks_overwrite();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
