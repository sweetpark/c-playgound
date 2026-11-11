/* P061 nullterm — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <string.h>
#include "raw_copy.h"

void copy_raw_bad(const char *raw, char *out)
{
    strcpy(out, raw);
}

int copy_raw_good(const char *raw, size_t raw_len, char *out, size_t out_size)
{
	(void)raw;
	(void)raw_len;
	(void)out;
	(void)out_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

