#include <errno.h>
#include <stddef.h>
#include <stdio.h>

/* P016 create_new — 모범답안 */
int create_new(const char *path)
{
	FILE *fp;

	if (path == NULL)
		return -1;
	fp = fopen(path, "wx");            /* 존재 확인과 생성이 원자적 (access 후 fopen 은 TOCTOU) */
	if (fp == NULL)
		return (errno == EEXIST) ? 1 : -1;
	if (fclose(fp) != 0)
		return -1;
	return 0;
}
