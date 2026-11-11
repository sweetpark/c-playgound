/* P099 dnstrust — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
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
	(void)claimed_ip;
	(void)out_ip;
	(void)out_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

