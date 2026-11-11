# HINT2 (막혔을 때만)
```
int ldap_lookup_bad(const char *name)
{
    /* 1. char filter[64]
     * 2. snprintf(filter, sizeof(filter), "(name=%s)", name)   ← 검증 없이 조립
     * 3. return mock_ldap_search(filter) */
}

int ldap_lookup_good(const char *name)
{
    /* [ ] 1. 지역변수
     *        char filter[64] */

    /* [ ] 2. 검증 함수 is_safe_ldap_value(s) 를 먼저 만든다
     *        길이가 0 이거나 32 초과              → 0
     *        isalnum((unsigned char)c) 가 아님     → 0
     *        그 외                                 → 1 */

    /* [ ] 3. 검증 (조립 전)
     *        실패 → return -1 */

    /* [ ] 4. snprintf 로 "(name=%s)" 조립 */

    /* [ ] 5. return mock_ldap_search(filter) */
}
```
