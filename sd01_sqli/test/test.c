#include <stdio.h>
#include "sql_auth.h"

static int g_fail = 0;
#define T_TRUE(cond, msg)                                                \
    do { if (!(cond)) { g_fail++;                                        \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_is_vulnerable(void)     /* Bad의 결함이 실제로 있어야 한다 */
{
    T_TRUE(login_bad("alice", "x' OR '1'='1") == 0,
        "Bad는 OR 트릭으로 인증이 우회되어야 한다(취약점 재현 확인)");
}

static void test_bad_normal_login(void)      /* Bad도 정상 케이스는 원래 동작해야 한다 */
{
    T_TRUE(login_bad("alice", "pw123!") == 0, "정상 로그인 성공");
    T_TRUE(login_bad("alice", "wrong")  == -1, "오답 처리 실패");
}

static void test_good_blocks_injection(void) /* Good은 같은 공격을 막아야 한다 */
{
    T_TRUE(login_good("alice", "x' OR '1'='1") == -1,
        "Good은 같은 공격 입력을 막아야 한다");
}

static void test_good_normal_login(void)     /* Good이 정상까지 막으면(과잉 차단) 실패 */
{
    T_TRUE(login_good("alice", "pw123!") == 0, "Good도 정상 로그인은 통과");
    T_TRUE(login_good("bob", "hunter2")  == 1, "두 번째 회원도 통과");
}

int main(void)
{
    test_bad_is_vulnerable();
    test_bad_normal_login();
    test_good_blocks_injection();
    test_good_normal_login();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
