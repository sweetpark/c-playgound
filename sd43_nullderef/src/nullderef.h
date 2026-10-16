#ifndef NULLDEREF_H
#define NULLDEREF_H
#include <stddef.h>
const char *lookup_env_like(const char *key);
void copy_env_bad(const char *key, char *out, size_t out_size);
int copy_env_good(const char *key, char *out, size_t out_size);
#endif
