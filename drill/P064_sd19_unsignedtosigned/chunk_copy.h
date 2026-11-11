#ifndef CHUNK_COPY_H
#define CHUNK_COPY_H
#include <stddef.h>
int get_chunk_size(int initialized, int chunk_size);
void chunk_copy_bad(void *dst, const void *src, int initialized, int chunk_size);
int chunk_copy_good(void *dst, size_t dst_size, const void *src, int initialized, int chunk_size);
#endif
