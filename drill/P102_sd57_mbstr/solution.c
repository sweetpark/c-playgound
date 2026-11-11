/* P102 mbstr — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <string.h>
#include "mbstr.h"

void mb_copy_bad(char *dst, const char *src)
{
    strcpy(dst, src);                  /* 크기 인자 자체가 없음 -- _mbscpy와 동일한 결함 */
}

int mb_copy_good(char *dst, size_t dst_size, const char *src)
{
	(void)dst;
	(void)dst_size;
	(void)src;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

