/* P109 shed — 요구사항은 problem.md. shed.h 의 선언을 보고 채운다. */
#include <stdlib.h>

#include "shed.h"

struct shed {
	/* TODO: 한도, 지금 구간 번호, 이 구간에서 받은 수, 받은/거절한 누계 */
	int unused;
};

shed_t *shed_create(int tps_limit)
{
	(void)tps_limit;
	/* TODO */
	return NULL;
}

int shed_admit(shed_t *s, long now_ms)
{
	(void)s;
	(void)now_ms;
	/* TODO */
	return -1;
}

void shed_stats(const shed_t *s, long *admitted, long *rejected)
{
	(void)s;
	(void)admitted;
	(void)rejected;
	/* TODO */
}

void shed_destroy(shed_t *s)
{
	(void)s;
	/* TODO */
}

int shed_cut_percent(int cpu_pct)
{
	(void)cpu_pct;
	/* TODO */
	return -1;
}

int shed_pick_reject(int cut_percent, unsigned seq)
{
	(void)cut_percent;
	(void)seq;
	/* TODO */
	return -1;
}

long backoff_ms(int attempt, long base_ms, long cap_ms, int jitter_pct, unsigned rnd)
{
	(void)attempt;
	(void)base_ms;
	(void)cap_ms;
	(void)jitter_pct;
	(void)rnd;
	/* TODO */
	return -1;
}
