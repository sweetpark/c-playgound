#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* P037 join_strs — 모범답안 */
int join_strs(char *out, size_t outsize, const char *const *items, size_t cnt, const char *sep)
{
	size_t pos = 0;
	size_t i;

	if (out == NULL || outsize == 0 || sep == NULL || (items == NULL && cnt > 0))
		return -1;
	for (i = 0; i < cnt; i++)
		if (items[i] == NULL)
			return -1;                       /* 쓰기 전에 모든 인자를 검증 */

	out[0] = '\0';
	for (i = 0; i < cnt; i++) {
		/* 남은 크기(outsize - pos)를 매번 넘기고, 반환값은 "필요했을 길이" 이므로 남은 크기와 비교 */
		int n = snprintf(out + pos, outsize - pos, "%s%s", (i > 0) ? sep : "", items[i]);
		if (n < 0 || (size_t)n >= outsize - pos) {
			out[0] = '\0';
			return -2;
		}
		pos += (size_t)n;
	}
	return 0;
}
