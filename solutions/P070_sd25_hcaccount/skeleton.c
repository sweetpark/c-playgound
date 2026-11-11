/* P070 hcaccount — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <stdio.h>
#include "db_login.h"

void db_login_bad(const char *passwd, char *out, size_t out_size)
{
    snprintf(out, out_size, "user=%s;pw=%s", "root", passwd);
}

void db_login_good(const char *user, const char *passwd, char *out, size_t out_size)
{
	(void)user;
	(void)passwd;
	(void)out;
	(void)out_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	drill_todo_hit = 1; /* 구현하면 이 줄을 지운다 */
}

