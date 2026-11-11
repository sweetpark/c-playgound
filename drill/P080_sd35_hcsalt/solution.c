/* P080 hcsalt — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 
#endif
#include <string.h>
#include <sys/random.h>
#include "salt_gen.h"

unsigned get_salt_bad(void)
{
    return 0xC0FFEE;
}

int get_salt_good(unsigned *out_salt)
{
	(void)out_salt;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

