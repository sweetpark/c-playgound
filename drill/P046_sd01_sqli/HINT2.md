# HINT2 (막혔을 때만)
```
int login_bad(const char *userId, const char *password)
{
    /* 1. where[256] 에 "username='%s' AND password='%s'" 를 snprintf 로 조립한다.
     * 2. 검증 없이 mock_db_login(where) 를 호출하고 결과를 반환한다. */
}

int login_good(const char *userId, const char *password)
{
    /* [ ] 1. 지역변수
     *        char where[256] */

    /* [ ] 2. 검증 함수 is_safe_sql_literal(s, max_len) 를 먼저 만든다
     *        길이가 0 이거나 max_len 초과          → 0
     *        영숫자 또는 '!' 가 아닌 글자가 있음    → 0
     *        banned 문자열이 들어 있음              → 0
     *          { "select","delete","update","insert","or","and","--" }
     *        그 외                                  → 1 */

    /* [ ] 3. userId 검증 (조립 전)
     *        실패 → return -1 */

    /* [ ] 4. password 검증 (조립 전)
     *        실패 → return -1 */

    /* [ ] 5. where 조립 (Bad 와 똑같은 snprintf) */

    /* [ ] 6. return mock_db_login(where) */
}
```
