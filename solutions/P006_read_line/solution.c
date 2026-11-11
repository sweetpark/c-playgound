#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* P006 read_line — 모범답안 */
int read_line(FILE *fp, char *buf, size_t size)
{
	size_t len = 0;
	size_t nread = 0;
	int truncated = 0;
	int c;                              /* char 가 아니라 int: EOF(-1) 와 0xFF 를 구분 */

	if (fp == NULL || buf == NULL || size == 0)
		return -2;

	while ((c = fgetc(fp)) != EOF && c != '\n') {
		nread++;
		if (len + 1 < size)             /* 마지막 한 칸은 널 문자용 */
			buf[len++] = (char)c;
		else
			truncated = 1;              /* 읽어서 버린다 */
	}
	buf[len] = '\0';

	if (c == EOF && nread == 0)
		return -1;
	return truncated ? 1 : 0;
}
