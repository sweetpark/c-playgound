#include "info_lookup.h"

int lookup_info_bad(int id, const int *info)
{
    short s;
    unsigned sz;
    s = (short)id;
    if (s > 256) return -1;
    sz = (unsigned)s;
    return info[sz];
}

int lookup_info_good(int id, const int *info, size_t info_size)
{
    if (id < 0 || (size_t)id >= info_size) return -1;
    return info[id];
}
