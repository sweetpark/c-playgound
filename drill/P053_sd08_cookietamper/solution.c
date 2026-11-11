/* P053 cookietamper — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <string.h>
#include "mock_session.h"
#include "admin_check.h"

int is_admin_bad(const char *cookie_role)
{
    return (cookie_role != NULL && strcmp(cookie_role, "admin") == 0);
}

int is_admin_good(const char *cookie_session_id)
{
	(void)cookie_session_id;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

