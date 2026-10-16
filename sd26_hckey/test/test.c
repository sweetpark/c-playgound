#include <stdio.h>
#include <string.h>
#include "mock_keystore.h"
#include "key_load.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    const unsigned char *k1 = get_key_bad();
    const unsigned char *k2 = get_key_bad();
    T_TRUE(k1 == k2 && memcmp(k1, k2, 16) == 0,
        "Bad는 언제 호출해도 완전히 같은 키를 반환해야 한다(교체 불가능함 = 취약점 재현)");
}

static void test_good_can_rotate_key(void)
{
    unsigned char keyA[16] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
    unsigned char keyB[16] = { 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 };
    unsigned char outA[16], outB[16];

    mock_keystore_register("db-enc-key-v1", keyA, sizeof(keyA));
    mock_keystore_register("db-enc-key-v2", keyB, sizeof(keyB));

    T_TRUE(get_key_good("db-enc-key-v1", outA, sizeof(outA)) == 0, "v1 키 로드는 성공해야 한다");
    T_TRUE(get_key_good("db-enc-key-v2", outB, sizeof(outB)) == 0, "v2 키 로드는 성공해야 한다");
    T_TRUE(memcmp(outA, outB, 16) != 0,
        "Good은 키 이름만 바꾸면 완전히 다른 키를 가져올 수 있어야 한다(키 교체 가능함을 증명)");
}

static void test_good_rejects_unknown_key(void)
{
    unsigned char out[16];
    T_TRUE(get_key_good("no-such-key", out, sizeof(out)) == -1,
        "Good은 등록되지 않은 키 이름은 실패로 알려야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_good_can_rotate_key();
    test_good_rejects_unknown_key();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
