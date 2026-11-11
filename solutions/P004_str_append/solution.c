#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* P004 str_append — 모범답안 */
int str_append(char *dst, size_t dstsize, const char *src)
{
	size_t dlen, slen;

	if (dst == NULL || src == NULL || dstsize == 0)
		return -1;

	dlen = strnlen(dst, dstsize);
	if (dlen == dstsize)               /* 범위 안에 널 문자 없음 */
		return -1;

	slen = strlen(src);
	if (slen > dstsize - dlen - 1)     /* dlen < dstsize 이므로 뺄셈 안전 */
		return -1;

	memcpy(dst + dlen, src, slen + 1);
	return 0;
}
