#include <stdio.h>
#include <string.h>
#include "dnstrust.h"

const char *resolve_hostname_like(const char *hostname, int poisoned)
{
    if (strcmp(hostname, "www.trust.com") != 0) return NULL;
    return poisoned ? "198.51.100.66" : "203.0.113.10";
}

int decide_send_target_bad(const char *hostname, int poisoned, char *out_ip, size_t out_size)
{
    const char *resolved;
    if (strcmp(hostname, "www.trust.com") != 0) return -1;   /* 도메인명만 확인 */
    resolved = resolve_hostname_like(hostname, poisoned);
    if (!resolved) return -1;
    snprintf(out_ip, out_size, "%s", resolved);                /* DNS가 알려준 IP를 그대로 신뢰 */
    return 0;
}

int decide_send_target_good(const char *claimed_ip, char *out_ip, size_t out_size)
{
    if (strcmp(claimed_ip, "203.0.113.10") != 0) return -1;    /* IP 화이트리스트 직접 비교 */
    snprintf(out_ip, out_size, "%s", claimed_ip);
    return 0;
}
