#ifndef ARR_WRITE_H
#define ARR_WRITE_H
#include <stddef.h>
void write_at_index_bad(int *arr, int num, int value);
int write_at_index_good(int *arr, size_t arr_size, int num, int value);
#endif
