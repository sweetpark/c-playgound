/* P065 idor — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <string.h>
#include "mock_login.h"
#include "record_access.h"

int view_record_bad(const char *sid, const char *targetUsername)
{
    const char *loginUser = mock_login_lookup_user(sid);
    (void)targetUsername;
    return (loginUser != NULL);
}

int view_record_good(const char *sid, const char *targetUsername)
{
	(void)sid;
	(void)targetUsername;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

