#ifndef BUF_OPS_H
#define BUF_OPS_H
#include <stddef.h>
void manipulate_string_bad(const char *input);
int manipulate_string_good(const char *input, char *out, size_t out_size);
#endif
