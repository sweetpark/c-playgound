#ifndef MOCK_SESSION_H
#define MOCK_SESSION_H
typedef enum { ROLE_GUEST, ROLE_USER, ROLE_ADMIN } role_t;
/* 세션ID로 등록된 역할을 조회. 등록 안 된 세션이면 ROLE_GUEST */
role_t mock_session_lookup_role(const char *session_id);
/* 테스트용: 세션을 등록(서버가 로그인 시점에 하는 일을 흉내) */
void mock_session_register(const char *session_id, role_t role);
#endif
