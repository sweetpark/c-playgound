/* P058 writebeforestart — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include "array_ops.h"
#define MAX_WORDS 10

void write_before_start_bad(int count)
{
    int words[MAX_WORDS];
    int wordIndex = MAX_WORDS - 1;
    int i;
    for (i = 0; i < count; i++) {
        words[wordIndex] = i;
        wordIndex--;
    }
    (void)words;
}

int write_before_start_good(int count, int *out_arr, size_t arr_size)
{
	(void)count;
	(void)out_arr;
	(void)arr_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

