/* P051 pathtraversal — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 
#endif
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
	(void)sandbox_root;
	(void)rName;
	(void)out;
	(void)out_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

