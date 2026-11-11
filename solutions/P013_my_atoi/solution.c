#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

/* P013 my_atoi — 모범답안 */
int my_atoi(const char *s, int *out)
{
	char *end;
	long v;

	if (s == NULL || out == NULL || s[0] == '\0')
		return -1;
	if (s[0] == ' ' || s[0] == '\t' || s[0] == '\n')   /* strtol 이 건너뛰는 앞 공백은 거절 */
		return -1;

	errno = 0;                                /* 호출 전 초기화: 이전 값이 남아 있을 수 있다 */
	v = strtol(s, &end, 10);
	if (end == s || *end != '\0')             /* 숫자 없음 / 뒤쪽 쓰레기 */
		return -1;
	if (errno == ERANGE || v > INT_MAX || v < INT_MIN)
		return -2;

	*out = (int)v;
	return 0;
}
