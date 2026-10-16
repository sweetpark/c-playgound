#include <stdio.h>
#include <string.h>
#include "dnstrust.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static void test_bad_normal_without_poisoning(void)
{
    char out[64];
    T_TRUE(decide_send_target_bad("www.trust.com", 0, out, sizeof(out)) == 0, "포이즈닝이 없으면 정상 전송돼야 한다");
    T_TRUE(strcmp(out, "203.0.113.10") == 0, "정상 상황에서는 진짜 신뢰 IP로 전송돼야 한다");
}

static void test_bad_is_vulnerable_to_poisoning(void)
{
    char out[64];
    T_TRUE(decide_send_target_bad("www.trust.com", 1 /* 포이즈닝됨 */, out, sizeof(out)) == 0,
        "Bad는 DNS가 포이즈닝돼도 도메인명만 보고 여전히 '신뢰함'으로 판정해야 한다(취약점 재현)");
    T_TRUE(strcmp(out, "198.51.100.66") == 0,
        "그 결과 데이터가 공격자 IP로 전송되는 목적지가 설정돼야 한다(공격 성공 재현)");
}

static void test_good_accepts_whitelisted_ip(void)
{
    char out[64];
    T_TRUE(decide_send_target_good("203.0.113.10", out, sizeof(out)) == 0, "화이트리스트에 있는 IP는 통과해야 한다");
    T_TRUE(strcmp(out, "203.0.113.10") == 0, "전송 목적지가 정확해야 한다");
}

static void test_good_rejects_attacker_ip(void)
{
    char out[64];
    T_TRUE(decide_send_target_good("198.51.100.66", out, sizeof(out)) == -1,
        "Good은 DNS 이름을 아예 보지 않고 IP 자체를 비교하므로 공격자 IP를 거부해야 한다");
}

int main(void)
{
    test_bad_normal_without_poisoning();
    test_bad_is_vulnerable_to_poisoning();
    test_good_accepts_whitelisted_ip();
    test_good_rejects_attacker_ip();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
