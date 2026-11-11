/* P055 proccontrol — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <stdio.h>
#include <string.h>
#include "doc_convert.h"

int build_convert_command_bad(const char *op, const char *documentName, char *out, size_t out_size)
{
    int n = snprintf(out, out_size, "%s %s", op, documentName);
    return (n >= 0 && (size_t)n < out_size) ? 0 : -1;
}

typedef struct { const char *op; const char *command; } op_map_t;

#define OP_TABLE_SIZE (sizeof(OP_TABLE)/sizeof(OP_TABLE[0]))

int build_convert_command_good(const char *op, const char *documentName, char *out, size_t out_size)
{
	(void)op;
	(void)documentName;
	(void)out;
	(void)out_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

