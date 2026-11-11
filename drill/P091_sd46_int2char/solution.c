/* P091 int2char — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include "int2char.h"

void store_offset_bad(doc_bad_t *doc, int line, int offset)
{
    doc->offsets_bad[line] = (char)offset;      /* 255 초과 시 잘림 */
}

int load_offset_bad(doc_bad_t *doc, int line)
{
    return (int)(unsigned char)doc->offsets_bad[line];
}

void store_offset_good(doc_good_t *doc, int line, int offset)
{
	(void)doc;
	(void)line;
	(void)offset;
	/* TODO: problem.md 를 보고 Good 구현 */
	drill_todo_hit = 1; /* 구현하면 이 줄을 지운다 */
}

int load_offset_good(doc_good_t *doc, int line)
{
	(void)doc;
	(void)line;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

