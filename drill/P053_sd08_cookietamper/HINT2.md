# HINT2 (막혔을 때만)
```
int is_admin_bad(const char *cookie_role)
{
    /* 1. return cookie_role != NULL && strcmp(cookie_role, "admin") == 0
     *    ← 클라이언트가 보낸 값으로 직접 판단 */
}

int is_admin_good(const char *cookie_session_id)
{
    /* [ ] 1. 매개변수 의미를 바꾼다
     *        role 이 아니라 session id 만 받는다 */

    /* [ ] 2. 서버 저장소에서 역할을 조회한다
     *        mock_session_lookup_role(cookie_session_id) */

    /* [ ] 3. return (조회된 역할 == ROLE_ADMIN)
     *        없는 세션 / NULL 은 ROLE_ADMIN 이 아니다 */
}
```
