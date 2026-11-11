/* P069 hcpasswd — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <stdio.h>
#include "db_conn.h"

void db_connect_bad(const char *server, const char *user, char *out, size_t out_size)
{
    snprintf(out, out_size, "server=%s;user=%s;pw=%s", server, user, "asdf");
}

void db_connect_good(const char *server, const char *user, const char *passwd, char *out, size_t out_size)
{
	(void)server;
	(void)user;
	(void)passwd;
	(void)out;
	(void)out_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	drill_todo_hit = 1; /* 구현하면 이 줄을 지운다 */
}

