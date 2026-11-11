#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include "int_alloc.h"

#define RESERVE 8

void *int_alloc_bad(int count)
{
    int total = count + RESERVE;
    return malloc((size_t)total * sizeof(int));
}

void use_buffer_bad(int count)
{
    int *buf = (int *)int_alloc_bad(count);
    buf[0] = 42;               /* 반환값 검사 없음 */
    free(buf);
}

void *int_alloc_good(int count)
{
    int total;
    if (count < 0 || count > INT_MAX - RESERVE) return NULL;
    total = count + RESERVE;
    if ((size_t)total > SIZE_MAX / sizeof(int)) return NULL;
    return malloc((size_t)total * sizeof(int));
}

void *use_buffer_good(int count)
{
    int *buf = (int *)int_alloc_good(count);
    if (buf == NULL) return NULL;
    buf[0] = 42;
    return buf;
}
