/* P067 privgrant — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <stddef.h>
#include "uid_resolve.h"

int resolve_uid_bad(int isCorrectUser)
{
    if (isCorrectUser) return 0;
    return -1;
}

typedef struct { group_t grp; int uid; } grp_uid_t;

#define TBL_SIZE (sizeof(TBL)/sizeof(TBL[0]))

int resolve_uid_good(group_t group)
{
	(void)group;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

