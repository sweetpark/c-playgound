/* P076 weakrandom — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 
#endif
#include <stdlib.h>
#include <stdio.h>
#include <sys/random.h>
#include "token_gen.h"

void gen_token_bad(unsigned seed, char *out, size_t out_size)
{
    size_t i;
    srand(seed);
    for (i = 0; i + 1 < out_size; i++)
        out[i] = "0123456789abcdef"[rand() % 16];
    out[i] = '\0';
}

int gen_token_good(char *out, size_t out_size)
{
	(void)out;
	(void)out_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

