#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* P005 get_dirname — 모범답안 */
int get_dirname(const char *path, char *out, size_t outsize)
{
	const char *slash;
	size_t len;

	if (path == NULL || out == NULL || outsize == 0)
		return -1;

	slash = strrchr(path, '/');
	if (slash == NULL)
		return -1;

	len = (size_t)(slash - path);
	if (len == 0)                  /* 루트 바로 아래: "/x" -> "/" */
		len = 1;
	if (len + 1 > outsize)         /* 널 문자 자리까지 */
		return -1;

	memcpy(out, path, len);
	out[len] = '\0';
	return 0;
}
