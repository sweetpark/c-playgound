/* P072 commentleak — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 
#endif
#include <string.h>
#include <stddef.h>
#include "release_check.h"

#define KEYWORD_COUNT (sizeof(KEYWORDS) / sizeof(KEYWORDS[0]))

int release_ok_bad(const char *source_code) { (void)source_code; return 1; }
int release_ok_good(const char *source_code)
{
	(void)source_code;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

