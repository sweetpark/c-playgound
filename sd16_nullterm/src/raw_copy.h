#ifndef RAW_COPY_H
#define RAW_COPY_H
#include <stddef.h>
void copy_raw_bad(const char *raw, char *out);
int copy_raw_good(const char *raw, size_t raw_len, char *out, size_t out_size);
#endif
