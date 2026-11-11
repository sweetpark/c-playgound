# HINT2 (막혔을 때만)
```
int view_record_bad(const char *sid, const char *targetUsername)
{
    /* 1. loginUser = mock_login_lookup_user(sid)
     * 2. (void)targetUsername   ← 대상은 쳐다보지도 않는다
     * 3. return (loginUser != NULL)   ← 로그인만 했으면 허용 */
}

int view_record_good(const char *sid, const char *targetUsername)
{
    /* [ ] 1. 세션에서 진짜 신원을 꺼낸다
     *        loginUser = mock_login_lookup_user(sid) */

    /* [ ] 2. 세션이 없으면 → return 0   (인증 실패) */

    /* [ ] 3. 요청 대상과 로그인 사용자를 대조한다
     *        return (strcmp(loginUser, targetUsername) == 0)   (인가) */
}
```
