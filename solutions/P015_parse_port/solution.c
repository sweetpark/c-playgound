#include <ctype.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

/* P015 parse_port — 모범답안 */
int parse_port(const char *s, unsigned short *out)
{
	char *end;
	unsigned long v;

	if (s == NULL || out == NULL)
		return -1;
	if (!isdigit((unsigned char)s[0]))        /* 부호·공백 거절 (strtoul 은 "-1" 을 받아들인다) */
		return -1;

	errno = 0;
	v = strtoul(s, &end, 10);
	if (end == s || *end != '\0')
		return -1;
	if (errno == ERANGE || v < 1 || v > 65535)
		return -1;

	*out = (unsigned short)v;
	return 0;
}
