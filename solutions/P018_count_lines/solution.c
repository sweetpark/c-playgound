#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* P018 count_lines — 모범답안 */
long count_lines(const char *path)
{
	FILE *fp;
	char buf[16];                  /* 일부러 작다: 한 줄이 여러 번에 나뉘어 읽힌다 */
	long lines = 0;
	int at_line_start = 1;
	int err;

	if (path == NULL)
		return -1;
	fp = fopen(path, "r");
	if (fp == NULL)
		return -1;

	while (fgets(buf, sizeof(buf), fp) != NULL) {
		size_t n = strlen(buf);
		if (n > 0 && buf[n - 1] == '\n') {
			lines++;               /* 줄 끝을 본 것만 센다 */
			at_line_start = 1;
		} else {
			at_line_start = 0;     /* 줄 중간까지만 읽음 */
		}
	}
	err = ferror(fp);              /* 루프가 끝난 이유: EOF 인가 오류인가 */
	fclose(fp);                    /* 모든 경로에서 닫는다 */
	if (err)
		return -2;
	if (!at_line_start)
		lines++;                   /* 개행 없는 마지막 줄 */
	return lines;
}
