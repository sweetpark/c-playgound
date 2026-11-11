/* P063 signext — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include "info_lookup.h"

int lookup_info_bad(int id, const int *info)
{
    short s;
    unsigned sz;
    s = (short)id;
    if (s > 256) return -1;
    sz = (unsigned)s;
    return info[sz];
}

int lookup_info_good(int id, const int *info, size_t info_size)
{
	(void)id;
	(void)info;
	(void)info_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

