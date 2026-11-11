/* P048 xss — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <stdio.h>
#include <string.h>
#include "html_escape.h"

void render_title_bad(const char *title, char *out, size_t out_size)
{
    snprintf(out, out_size, "<FONT SIZE=+1><STRONG>%s</STRONG></FONT>", title);
}

void render_title_good(const char *title, char *out, size_t out_size)
{
	(void)title;
	(void)out;
	(void)out_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	drill_todo_hit = 1; /* 구현하면 이 줄을 지운다 */
}

