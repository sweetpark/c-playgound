#include <stdio.h>
#include <string.h>
#include "html_escape.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    char out[256];
    render_title_bad("<script>alert(1)</script>", out, sizeof(out));
    T_TRUE(strstr(out, "<script>") != NULL,
        "Bad는 <script> 태그가 그대로 출력에 살아있어야 한다(취약점 재현)");
}

static void test_bad_normal(void)
{
    char out[256];
    render_title_bad("Hello World", out, sizeof(out));
    T_TRUE(strstr(out, "Hello World") != NULL, "정상 제목은 그대로 출력");
}

static void test_good_blocks_xss(void)
{
    char out[256];
    render_title_good("<script>alert(1)</script>", out, sizeof(out));
    T_TRUE(strstr(out, "<script>") == NULL, "Good은 <script> 태그가 남아있으면 안 된다");
    T_TRUE(strstr(out, "&lt;script&gt;") != NULL, "Good은 이스케이프된 형태로 남아야 한다");
}

static void test_good_normal(void)
{
    char out[256];
    render_title_good("Hello World", out, sizeof(out));
    T_TRUE(strstr(out, "Hello World") != NULL, "Good도 평범한 제목은 그대로 출력");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_blocks_xss();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
