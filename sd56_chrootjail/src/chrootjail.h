#ifndef CHROOTJAIL_H
#define CHROOTJAIL_H
#include <stddef.h>
int resolve_in_jail_bad(const char *stale_cwd, const char *user_path, char *out, size_t out_size);
int resolve_in_jail_good(const char *jail_root, const char *user_path, char *out, size_t out_size);
#endif
