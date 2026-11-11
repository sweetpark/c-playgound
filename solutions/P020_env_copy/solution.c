#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* P020 env_copy — 모범답안 */
int env_copy(const char *name, char *out, size_t outsize)
{
	const char *v;
	size_t n;

	if (name == NULL || out == NULL || outsize == 0)
		return -1;
	v = getenv(name);                 /* 읽기 전용, 다음 getenv/setenv 로 바뀔 수 있다 */
	if (v == NULL)
		return -1;
	n = strlen(v);
	if (n + 1 > outsize)              /* 외부 값은 길이를 믿을 수 없다 */
		return -2;
	memcpy(out, v, n + 1);
	return 0;
}
