/* P096 unbounded — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include "unbounded.h"

static int g_active = 0;

int active_count(void) { return g_active; }
void reset_active(void) { g_active = 0; }

int handle_connection_bad(void)
{
    g_active++;                        /* 접속마다 상한 없이 생성 */
    return 0;
}

int handle_connection_good(void)
{
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

