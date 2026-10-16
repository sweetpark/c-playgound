#include "array_ops.h"
#define MAX_WORDS 10

void write_before_start_bad(int count)
{
    int words[MAX_WORDS];
    int wordIndex = MAX_WORDS - 1;
    int i;
    for (i = 0; i < count; i++) {
        words[wordIndex] = i;
        wordIndex--;
    }
    (void)words;
}

int write_before_start_good(int count, int *out_arr, size_t arr_size)
{
    int wordIndex;
    int i;
    if (count < 0) return -1;
    wordIndex = (int)arr_size - 1;
    for (i = 0; i < count; i++) {
        if (wordIndex < 0) return -1;
        out_arr[wordIndex] = i;
        wordIndex--;
    }
    return 0;
}
