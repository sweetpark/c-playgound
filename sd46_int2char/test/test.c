#include <stdio.h>
#include <string.h>
#include "int2char.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_truncates(void)
{
    doc_bad_t doc;
    memset(&doc, 0, sizeof(doc));
    int big_offset = 1000;   /* char 범위(255)를 훨씬 초과 */

    store_offset_bad(&doc, 0, big_offset);
    int got = load_offset_bad(&doc, 0);

    T_TRUE(got != big_offset,
        "Bad는 char 배열에 저장하면서 1000이 잘려 원래 값과 달라져야 한다(취약점 재현)");
    T_TRUE(got == (big_offset % 256),
        "Bad에서 손실된 값은 하위 8비트(모듈로 256)만 남아야 한다");
}

static void test_bad_normal_small_value(void)
{
    doc_bad_t doc;
    memset(&doc, 0, sizeof(doc));
    store_offset_bad(&doc, 0, 100);
    T_TRUE(load_offset_bad(&doc, 0) == 100, "255 이하 값은 Bad에서도 그대로 보존돼야 한다");
}

static void test_good_preserves_value(void)
{
    doc_good_t doc;
    memset(&doc, 0, sizeof(doc));
    int big_offset = 1000;

    store_offset_good(&doc, 0, big_offset);
    int got = load_offset_good(&doc, 0);

    T_TRUE(got == big_offset, "Good은 int 배열에 저장하므로 값이 그대로 보존돼야 한다");
}

int main(void)
{
    test_bad_truncates();
    test_bad_normal_small_value();
    test_good_preserves_value();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
