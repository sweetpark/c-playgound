/* P046 sqli — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mock_db.h"
#include "sql_auth.h"

#define MAX_USER_ID_LENGTH   8
#define MAX_PASSWORD_LENGTH 16

/* 취약: 사용자 입력을 검증 없이 그대로 WHERE 절에 이어붙인다 */
int login_bad(const char *userId, const char *password)
{
    char where[256];
    snprintf(where, sizeof(where),
        "username='%s' AND password='%s'", userId, password);
    return mock_db_login(where);
}

/* 영숫자만 허용(화이트리스트) + 길이 상한. 예약어 검사는 이중 방어용 */

int login_good(const char *userId, const char *password)
{
	(void)userId;
	(void)password;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

