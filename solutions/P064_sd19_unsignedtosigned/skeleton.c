/* P064 unsignedtosigned — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <string.h>
#include "chunk_copy.h"

int get_chunk_size(int initialized, int chunk_size)
{
    if (!initialized) return -1;
    return chunk_size;
}

void chunk_copy_bad(void *dst, const void *src, int initialized, int chunk_size)
{
    unsigned size;
    size = (unsigned)get_chunk_size(initialized, chunk_size);
    memcpy(dst, src, size);
}

int chunk_copy_good(void *dst, size_t dst_size, const void *src, int initialized, int chunk_size)
{
	(void)dst;
	(void)dst_size;
	(void)src;
	(void)initialized;
	(void)chunk_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

