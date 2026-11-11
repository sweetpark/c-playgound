/* P082 toctou — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 
#endif
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include "toctou.h"

int file_operation_bad(const char *path, const char *content)
{
    FILE *f;
    if (access(path, W_OK) != 0) return -1;      /* 검사 시점(TOC) */
    f = fopen(path, "w+");                        /* 사용 시점(TOU) -- 그 사이 이름이 다른 대상을 가리켜도 그대로 따라감 */
    if (!f) return -1;
    fputs(content, f);
    fclose(f);
    return 0;
}

int file_operation_good(const char *path, const char *content)
{
	(void)path;
	(void)content;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

