#include <stddef.h>
#include "uid_resolve.h"

int resolve_uid_bad(int isCorrectUser)
{
    if (isCorrectUser) return 0;
    return -1;
}

typedef struct { group_t grp; int uid; } grp_uid_t;
static const grp_uid_t TBL[] = {
    { GRP_ADMIN, 0 }, { GRP_NORMAL, 1000 }, { GRP_GUEST, 1001 },
};
#define TBL_SIZE (sizeof(TBL)/sizeof(TBL[0]))

int resolve_uid_good(group_t group)
{
    size_t i;
    for (i = 0; i < TBL_SIZE; i++)
        if (TBL[i].grp == group) return TBL[i].uid;
    return -1;
}
