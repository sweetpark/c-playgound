/* P077 pwplain — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <string.h>
#include "pw_store.h"

void save_password_bad(const char *pw, char *stored, size_t size)
{
    strncpy(stored, pw, size - 1);
    stored[size - 1] = '\0';
}

int verify_password_bad(const char *stored, const char *input)
{
    return strcmp(stored, input) == 0;
}

void save_password_good(const char *pw, unsigned *stored)
{
	(void)pw;
	(void)stored;
	/* TODO: problem.md 를 보고 Good 구현 */
	drill_todo_hit = 1; /* 구현하면 이 줄을 지운다 */
}

int verify_password_good(unsigned stored, const char *input)
{
	(void)stored;
	(void)input;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

