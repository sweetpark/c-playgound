# HINT2 (막혔을 때만)
```
int decide_send_target_bad(const char *hostname, int poisoned,
                           char *out_ip, size_t out_size)
{
    /* 1. hostname 이 "www.trust.com" 이 아니면 return -1   ← 도메인명만 확인
     * 2. resolved = resolve_hostname_like(hostname, poisoned)   NULL 이면 return -1
     * 3. snprintf(out_ip, out_size, "%s", resolved)   ← DNS 가 알려준 IP 를 그대로 신뢰
     * 4. return 0 */
}

int decide_send_target_good(const char *claimed_ip, char *out_ip, size_t out_size)
{
    /* [ ] 1. 시그니처에서 hostname, poisoned 를 **없앤다**
     *        DNS 는 판정 경로에 들어오지 않는다 */

    /* [ ] 2. IP 화이트리스트를 직접 비교
     *        strcmp(claimed_ip, "203.0.113.10") != 0 → return -1 */

    /* [ ] 3. snprintf(out_ip, out_size, "%s", claimed_ip) */

    /* [ ] 4. return 0 */
}
```
