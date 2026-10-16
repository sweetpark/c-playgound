#ifndef INFO_LOOKUP_H
#define INFO_LOOKUP_H
#include <stddef.h>
int lookup_info_bad(int id, const int *info);
int lookup_info_good(int id, const int *info, size_t info_size);
#endif
