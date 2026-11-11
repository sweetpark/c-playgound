/* P059 readoverflow — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include "arr_read.h"

int read_at_index_bad(const int *arr, int idx)
{
    return arr[idx];
}

int read_at_index_good(const int *arr, size_t size, int idx, int *out)
{
	(void)arr;
	(void)size;
	(void)idx;
	(void)out;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

