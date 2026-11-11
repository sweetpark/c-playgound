/* P084 symmap — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "symmap.h"

int open_predictable_tmp_bad(const char *dir, const char *content)
{
    char path[PATH_MAX];
    FILE *f;
    snprintf(path, sizeof(path), "%s/guess.tmp", dir);   /* 이름이 항상 같다 -> 예측 가능 */

    if (access(path, W_OK) != 0) return -1;
    f = fopen(path, "w+");                                /* 심볼릭 링크면 그대로 따라간다 */
    if (!f) return -1;
    fputs(content, f);
    fclose(f);
    return 0;
}

int create_random_tmp_good(const char *dir, char *out_path, size_t out_size, const char *content)
{
	(void)dir;
	(void)out_path;
	(void)out_size;
	(void)content;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

