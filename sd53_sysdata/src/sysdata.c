#include <stdio.h>
#include "sysdata.h"

void report_exe_not_found_bad(const char *path_env, char *out, size_t out_size)
{
    snprintf(out, out_size, "cannot find exe on path %s\n", path_env);
}

void report_exe_not_found_good(const char *path_env, char *out, size_t out_size)
{
    (void)path_env;
    snprintf(out, out_size, "cannot find exe on path \n");
}

void report_alloc_failure_bad(void *ptr, char *out, size_t out_size)
{
    snprintf(out, out_size, "malloc failed at address %p\n", ptr);
}

void report_alloc_failure_good(void *ptr, char *out, size_t out_size)
{
    (void)ptr;
    snprintf(out, out_size, "malloc failed\n");
}
