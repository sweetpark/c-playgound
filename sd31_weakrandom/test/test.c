#include <stdio.h>
#include <string.h>
#include "token_gen.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    char t1[33], t2[33];
    gen_token_bad(1, t1, sizeof(t1));
    gen_token_bad(1, t2, sizeof(t2));
    T_TRUE(strcmp(t1, t2) == 0,
        "Bad는 같은 seed면 항상 같은 토큰을 만들어야 한다(예측 가능함 = 취약점 재현)");
}

static void test_bad_normal(void)
{
    char t1[33], t2[33];
    gen_token_bad(1, t1, sizeof(t1));
    gen_token_bad(2, t2, sizeof(t2));
    T_TRUE(strcmp(t1, t2) != 0, "적어도 seed가 다르면 결과도 달라진다(그래도 예측 가능한 건 여전함)");
}

static void test_good_is_unpredictable(void)
{
    char t1[33], t2[33];
    T_TRUE(gen_token_good(t1, sizeof(t1)) == 0, "정상 크기 버퍼는 성공해야 한다");
    T_TRUE(gen_token_good(t2, sizeof(t2)) == 0, "두 번째 호출도 성공해야 한다");
    T_TRUE(strcmp(t1, t2) != 0,
        "Good은 매 호출마다 다른 토큰을 만들어야 한다(진짜 난수원을 쓴다는 증거)");
}

static void test_good_rejects_small_buffer(void)
{
    char tiny[4];
    T_TRUE(gen_token_good(tiny, sizeof(tiny)) == -1, "Good은 버퍼가 너무 작으면 실패로 알려야 한다");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal();
    test_good_is_unpredictable();
    test_good_rejects_small_buffer();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
