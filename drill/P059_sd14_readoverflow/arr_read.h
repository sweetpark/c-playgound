#ifndef ARR_READ_H
#define ARR_READ_H
#include <stddef.h>
int read_at_index_bad(const int *arr, int idx);
int read_at_index_good(const int *arr, size_t size, int idx, int *out);
#endif
