#ifndef MBSTR_H
#define MBSTR_H
#include <stddef.h>
void mb_copy_bad(char *dst, const char *src);
int  mb_copy_good(char *dst, size_t dst_size, const char *src);
#endif
