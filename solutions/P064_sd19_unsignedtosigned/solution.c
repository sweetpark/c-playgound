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
    int size = get_chunk_size(initialized, chunk_size);
    if (size < 0) return -1;
    if ((size_t)size > dst_size) return -1;
    memcpy(dst, src, (size_t)size);
    return 0;
}
