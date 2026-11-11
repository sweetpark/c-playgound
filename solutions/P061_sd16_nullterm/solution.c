#include <string.h>
#include "raw_copy.h"

void copy_raw_bad(const char *raw, char *out)
{
    strcpy(out, raw);
}

int copy_raw_good(const char *raw, size_t raw_len, char *out, size_t out_size)
{
    size_t n = (raw_len < out_size - 1) ? raw_len : out_size - 1;
    memcpy(out, raw, n);
    out[n] = '\0';
    return (raw_len < out_size) ? 0 : -1;
}
