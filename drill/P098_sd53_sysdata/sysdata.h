#ifndef SYSDATA_H
#define SYSDATA_H
#include <stddef.h>
void report_exe_not_found_bad(const char *path_env, char *out, size_t out_size);
void report_exe_not_found_good(const char *path_env, char *out, size_t out_size);
void report_alloc_failure_bad(void *ptr, char *out, size_t out_size);
void report_alloc_failure_good(void *ptr, char *out, size_t out_size);
#endif
