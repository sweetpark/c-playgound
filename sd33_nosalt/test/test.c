#include <stdio.h>
#include "salted_hash.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    unsigned h1 = hash_no_salt_bad("hunter2");
    unsigned h2 = hash_no_salt_bad("hunter2");
    T_TRUE(h1 == h2,
        "Bad는 두 사용자가 같은 패스워드를 쓰면 완전히 같은 해시가 나와야 한다(레인보우테이블 취약점 재현)");
}

static void test_good_blocks_precompute(void)
{
    unsigned h1 = hash_with_salt_good("hunter2", 111);
    unsigned h2 = hash_with_salt_good("hunter2", 222);
    T_TRUE(h1 != h2,
        "Good은 같은 패스워드라도 솔트가 다르면 해시도 달라져야 한다(사전 계산 공격 무력화)");
}

static void test_good_normal(void)
{
    unsigned h1 = hash_with_salt_good("hunter2", 111);
    unsigned h2 = hash_with_salt_good("hunter2", 111);
    T_TRUE(h1 == h2, "같은 패스워드+같은 솔트는 항상 같은 해시가 나와야 한다(검증 가능해야 함)");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_good_blocks_precompute();
    test_good_normal();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
