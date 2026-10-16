#include <stdio.h>
#include "salt_gen.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)
{
    unsigned s1 = get_salt_bad();
    unsigned s2 = get_salt_bad();
    T_TRUE(s1 == s2,
        "Bad는 언제 호출해도 완전히 같은 솔트를 반환해야 한다(레인보우테이블 재사용 가능 = 취약점 재현)");
}

static void test_good_produces_fresh_salt(void)
{
    unsigned s1 = 0, s2 = 0, s3 = 0;
    T_TRUE(get_salt_good(&s1) == 0, "첫 번째 호출은 성공해야 한다");
    T_TRUE(get_salt_good(&s2) == 0, "두 번째 호출도 성공해야 한다");
    T_TRUE(get_salt_good(&s3) == 0, "세 번째 호출도 성공해야 한다");
    T_TRUE(!(s1 == s2 && s2 == s3),
        "Good은 호출마다 매번 새 솔트를 만들어야 한다(세 값이 전부 같을 확률은 무시 가능)");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_good_produces_fresh_salt();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
