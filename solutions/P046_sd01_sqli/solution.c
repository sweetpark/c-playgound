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
static int is_safe_sql_literal(const char *s, size_t max_len)
{
    size_t i, len = strlen(s);
    static const char *banned[] =
        { "select","delete","update","insert","or","and","--" };
    size_t nb = sizeof(banned) / sizeof(banned[0]);

    if (len == 0 || len > max_len) return 0;

    for (i = 0; i < len; i++) {
        unsigned char c = (unsigned char)s[i];
        if (!isalnum(c) && c != '!') return 0;   /* 요구사항상 '!' 는 허용 문자 */
    }
    for (i = 0; i < nb; i++) {
        if (strstr(s, banned[i]) != NULL) return 0;
    }
    return 1;
}

int login_good(const char *userId, const char *password)
{
    char where[256];

    if (!is_safe_sql_literal(userId, MAX_USER_ID_LENGTH))    return -1;
    if (!is_safe_sql_literal(password, MAX_PASSWORD_LENGTH)) return -1;

    snprintf(where, sizeof(where),
        "username='%s' AND password='%s'", userId, password);
    return mock_db_login(where);
}
