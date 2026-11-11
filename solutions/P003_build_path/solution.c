#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* P003 build_path — 모범답안 */
int build_path(char *out, size_t outsize, const char *dir, const char *file)
{
	size_t dlen;
	const char *sep;
	int n;

	if (out == NULL || outsize == 0 || dir == NULL || file == NULL || file[0] == '\0')
		return -1;

	dlen = strlen(dir);
	sep = (dlen == 0 || dir[dlen - 1] == '/') ? "" : "/";

	n = snprintf(out, outsize, "%s%s%s", dir, sep, file);
	if (n < 0 || (size_t)n >= outsize) {
		out[0] = '\0';
		return -1;
	}
	return 0;
}
