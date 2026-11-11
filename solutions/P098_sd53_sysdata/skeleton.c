/* P098 sysdata — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <stdio.h>
#include "sysdata.h"

void report_exe_not_found_bad(const char *path_env, char *out, size_t out_size)
{
    snprintf(out, out_size, "cannot find exe on path %s\n", path_env);
}

void report_exe_not_found_good(const char *path_env, char *out, size_t out_size)
{
	(void)path_env;
	(void)out;
	(void)out_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	drill_todo_hit = 1; /* 구현하면 이 줄을 지운다 */
}

void report_alloc_failure_bad(void *ptr, char *out, size_t out_size)
{
    snprintf(out, out_size, "malloc failed at address %p\n", ptr);
}

void report_alloc_failure_good(void *ptr, char *out, size_t out_size)
{
	(void)ptr;
	(void)out;
	(void)out_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	drill_todo_hit = 1; /* 구현하면 이 줄을 지운다 */
}

