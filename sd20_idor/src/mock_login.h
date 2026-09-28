#ifndef MOCK_LOGIN_H
#define MOCK_LOGIN_H
/* 세션ID로 로그인한 사용자명을 조회. 등록 안 됐으면 NULL */
const char *mock_login_lookup_user(const char *sid);
void mock_login_register(const char *sid, const char *username);
#endif
