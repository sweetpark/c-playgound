#ifndef PATH_GUARD_H
#define PATH_GUARD_H
#include <stddef.h>
char *resolve_report_path_bad(const char *sandbox_root, const char *rName,
                               char *out, size_t out_size);
int resolve_report_path_good(const char *sandbox_root, const char *rName,
                              char *out, size_t out_size);
#endif
