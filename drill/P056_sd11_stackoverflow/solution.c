/* P056 stackoverflow — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <string.h>
#include "buf_ops.h"

void manipulate_string_bad(const char *input)
{
    char buf[24];
    strcpy(buf, input);      /* 길이 검사 없는 복사 */
    (void)buf;
}

int manipulate_string_good(const char *input, char *out, size_t out_size)
{
	(void)input;
	(void)out;
	(void)out_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

