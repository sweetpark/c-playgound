#include <stdio.h>
#include <string.h>
#include "secure_transmit.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    char out[64];
    prepare_for_send_bad("mypassword123", out, sizeof(out));
    T_TRUE(strcmp(out, "mypassword123") == 0,
        "Bad는 전송/저장 직전에도 원본 패스워드가 그대로 노출되어야 한다(취약점 재현)");
}

static void test_good_hides_plaintext(void)
{
    char out[64];
    prepare_for_send_good("mypassword123", out, sizeof(out));
    T_TRUE(strcmp(out, "mypassword123") != 0,
        "Good은 전송/저장되는 값이 원본과 달라야 한다(평문 노출 없음)");
}

static void test_good_is_reversible_by_the_right_party(void)
{
    char masked[64], restored[64];
    prepare_for_send_good("mypassword123", masked, sizeof(masked));
    undo_mask(masked, restored, sizeof(restored));
    T_TRUE(strcmp(restored, "mypassword123") == 0,
        "정당한 수신자는 원본을 복원할 수 있어야 한다(암호화의 목적은 은닉이지 손실이 아님)");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_good_hides_plaintext();
    test_good_is_reversible_by_the_right_party();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
