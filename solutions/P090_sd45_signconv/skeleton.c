/* P090 signconv — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 
#endif
#include <string.h>
#include "signconv.h"

#define BUFSIZE 256

unsigned int len_bad(const char *s)
{
    if (s == NULL) return (unsigned int)-1;   /* -1 -> 4294967295 */
    return (unsigned int)strnlen(s, BUFSIZE - 1);
}

unsigned int len_good(const char *s)
{
	(void)s;
	/* TODO: problem.md 를 보고 Good 구현 */
	return 0;
}

int within_len_bad(unsigned int idx, const char *s)
{
    return idx < len_bad(s);
}

int within_len_good(unsigned int idx, const char *s)
{
	(void)idx;
	(void)s;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

