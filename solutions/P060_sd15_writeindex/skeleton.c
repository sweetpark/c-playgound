/* P060 writeindex — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include "arr_write.h"

void write_at_index_bad(int *arr, int num, int value)
{
    arr[num - 1] = value;
}

int write_at_index_good(int *arr, size_t arr_size, int num, int value)
{
	(void)arr;
	(void)arr_size;
	(void)num;
	(void)value;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

