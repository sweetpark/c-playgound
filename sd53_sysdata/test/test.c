#include <stdio.h>
#include <string.h>
#include "sysdata.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_leaks_path(void)
{
    char out[256];
    report_exe_not_found_bad("/usr/bin:/home/user/.secret_bin", out, sizeof(out));
    T_TRUE(strstr(out, "/home/user/.secret_bin") != NULL,
        "Bad는 PATH 환경변수를 오류 메시지에 그대로 노출해야 한다(취약점 재현)");
}

static void test_good_hides_path(void)
{
    char out[256];
    report_exe_not_found_good("/usr/bin:/home/user/.secret_bin", out, sizeof(out));
    T_TRUE(strstr(out, "/home/user/.secret_bin") == NULL,
        "Good은 오류 메시지에서 내부 경로를 완전히 생략해야 한다");
    T_TRUE(strstr(out, "cannot find exe on path") != NULL,
        "Good도 사용자에게 무엇이 실패했는지는 알려야 한다(경로만 뺀다)");
}

static void test_bad_leaks_address(void)
{
    char out[256];
    void *fake_ptr = (void *)0x55d3a1b2c000UL;
    report_alloc_failure_bad(fake_ptr, out, sizeof(out));
    T_TRUE(strstr(out, "0x") != NULL,
        "Bad는 메모리 주소를 16진수로 그대로 노출해야 한다(ASLR/메모리 배치 정보 유출 재현)");
}

static void test_good_hides_address(void)
{
    char out[256];
    void *fake_ptr = (void *)0x55d3a1b2c000UL;
    report_alloc_failure_good(fake_ptr, out, sizeof(out));
    T_TRUE(strstr(out, "0x") == NULL,
        "Good은 오류 메시지에 메모리 주소를 절대 포함하면 안 된다");
}

int main(void)
{
    test_bad_leaks_path();
    test_good_hides_path();
    test_bad_leaks_address();
    test_good_hides_address();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
