/* P068 privdrop — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include "priv_ops.h"

#define NORMAL_UID 1000
#define ROOT_UID   0

static int g_current_priv = NORMAL_UID;

int get_current_priv(void) { return g_current_priv; }
void reset_priv(void) { g_current_priv = NORMAL_UID; }

void elevate_bad(void)
{
    g_current_priv = ROOT_UID;
}

void do_privileged_work_good(void)
{
	/* TODO: problem.md 를 보고 Good 구현 */
	drill_todo_hit = 1; /* 구현하면 이 줄을 지운다 */
}

