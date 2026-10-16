#ifndef SYMMAP_H
#define SYMMAP_H
#include <stddef.h>
int open_predictable_tmp_bad(const char *dir, const char *content);
int create_random_tmp_good(const char *dir, char *out_path, size_t out_size, const char *content);
#endif
