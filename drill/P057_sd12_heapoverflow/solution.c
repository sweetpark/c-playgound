/* P057 heapoverflow — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <stdlib.h>
#include <string.h>
#include "heap_ops.h"

#define BUFSIZE 10

void heap_copy_bad(const char *input)
{
    char *dest = (char *)malloc(BUFSIZE);
    if (dest == NULL) return;
    strcpy(dest, input);
    free(dest);
}

int heap_copy_good(const char *input, char **out)
{
	(void)input;
	(void)out;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

