#include <stdio.h>
#include <string.h>
#include "pw_store.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    char stored[32];
    save_password_bad("hunter2", stored, sizeof(stored));
    T_TRUE(strcmp(stored, "hunter2") == 0,
        "Bad는 저장된 값 자체가 원본 패스워드와 완전히 같아야 한다(평문 저장 재현)");
}

static void test_bad_normal(void)
{
    char stored[32];
    save_password_bad("hunter2", stored, sizeof(stored));
    T_TRUE(verify_password_bad(stored, "hunter2") == 1, "정상 인증은 통과해야 한다");
    T_TRUE(verify_password_bad(stored, "wrong") == 0, "틀린 패스워드는 실패해야 한다");
}

static void test_good_never_stores_plaintext(void)
{
    unsigned stored;
    save_password_good("hunter2", &stored);
    T_TRUE(stored != 0, "저장된 해시값이 존재해야 한다(타입 자체가 평문일 수 없음을 구조로 보증)");
}

static void test_good_normal(void)
{
    unsigned stored;
    save_password_good("hunter2", &stored);
    T_TRUE(verify_password_good(stored, "hunter2") == 1, "정상 인증은 통과해야 한다");
    T_TRUE(verify_password_good(stored, "wrong") == 0, "틀린 패스워드는 실패해야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_never_stores_plaintext();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
