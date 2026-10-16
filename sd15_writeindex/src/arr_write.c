#include "arr_write.h"

void write_at_index_bad(int *arr, int num, int value)
{
    arr[num - 1] = value;
}

int write_at_index_good(int *arr, size_t arr_size, int num, int value)
{
    if (num <= 0 || (size_t)num > arr_size) return -1;
    arr[num - 1] = value;
    return 0;
}
