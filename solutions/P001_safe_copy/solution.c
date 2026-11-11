#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* P001 safe_copy — 모범답안 */
int safe_copy(char *dst, size_t dstsize, const char *src)
{
	size_t n;

	if (dst == NULL || dstsize == 0 || src == NULL)
		return -1;

	n = strlen(src);
	if (n >= dstsize) {            /* 널 문자 자리까지 못 들어간다 */
		memcpy(dst, src, dstsize - 1);
		dst[dstsize - 1] = '\0';
		return 1;
	}
	memcpy(dst, src, n + 1);       /* 널 문자까지 */
	return 0;
}
