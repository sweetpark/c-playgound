#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "path_guard.h"

char *resolve_report_path_bad(const char *sandbox_root, const char *rName,
                               char *out, size_t out_size)
{
    if (strlen(sandbox_root) + 1 + strlen(rName) + 1 > out_size) return NULL;
    snprintf(out, out_size, "%s/%s", sandbox_root, rName);
    return out;
}

int resolve_report_path_good(const char *sandbox_root, const char *rName,
                              char *out, size_t out_size)
{
    char joined[PATH_MAX];
    char real_sandbox[PATH_MAX];
    char real_target[PATH_MAX];
    size_t sandbox_len;

    if (snprintf(joined, sizeof(joined), "%s/%s", sandbox_root, rName)
        >= (int)sizeof(joined)) return -1;

    if (realpath(sandbox_root, real_sandbox) == NULL) return -1;
    if (realpath(joined, real_target) == NULL) return -1;

    sandbox_len = strlen(real_sandbox);
    if (strncmp(real_target, real_sandbox, sandbox_len) != 0 ||
        (real_target[sandbox_len] != '/' && real_target[sandbox_len] != '\0')) {
        return -1;
    }

    if (out_size <= strlen(real_target)) return -1;
    snprintf(out, out_size, "%s", real_target);
    return 0;
}
