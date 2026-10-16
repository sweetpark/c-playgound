#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "debugcode.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static int capture_output_contains(void (*fn)(void), const char *needle)
{
    char buf[256];
    FILE *tmp;
    int saved_fd, found;
    long n;

    memset(buf, 0, sizeof(buf));
    tmp = tmpfile();
    if (!tmp) return -1;

    fflush(stdout);
    saved_fd = dup(fileno(stdout));
    dup2(fileno(tmp), fileno(stdout));

    fn();

    fflush(stdout);
    dup2(saved_fd, fileno(stdout));
    close(saved_fd);

    rewind(tmp);
    n = (long)fread(buf, 1, sizeof(buf) - 1, tmp);
    (void)n;
    fclose(tmp);

    found = strstr(buf, needle) != NULL;
    return found;
}

static void test_bad_always_prints(void)
{
    T_TRUE(capture_output_contains(debug_output_bad, "Debug message") == 1,
        "Bad는 DEBUG_ENABLE 여부와 무관하게 항상 디버그 메시지를 출력해야 한다(취약점 재현)");
}

static void test_good_silent_in_release_build(void)
{
    T_TRUE(capture_output_contains(debug_output_good, "Debug message") == 0,
        "Good은 DEBUG_ENABLE 없이 빌드(기본 릴리스 빌드)하면 아무 것도 출력하지 않아야 한다");
}

int main(void)
{
    test_bad_always_prints();
    test_good_silent_in_release_build();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
