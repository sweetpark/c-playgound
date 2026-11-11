/* P078 nosalt — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <stdio.h>
#include "salted_hash.h"

static unsigned toy_hash(const char *s)
{
    unsigned h = 5381;
    while (*s) h = ((h << 5) + h) + (unsigned char)(*s++);
    return h;
}

unsigned hash_no_salt_bad(const char *pw) { return toy_hash(pw); }

unsigned hash_with_salt_good(const char *pw, unsigned salt)
{
	(void)pw;
	(void)salt;
	/* TODO: problem.md 를 보고 Good 구현 */
	return 0;
}

