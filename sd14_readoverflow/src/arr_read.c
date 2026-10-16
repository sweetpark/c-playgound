#include "arr_read.h"

int read_at_index_bad(const int *arr, int idx)
{
    return arr[idx];
}

int read_at_index_good(const int *arr, size_t size, int idx, int *out)
{
    if (idx < 0 || (size_t)idx >= size) return -1;
    *out = arr[idx];
    return 0;
}
