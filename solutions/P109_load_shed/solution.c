/* P109 shed 모범답안 */
#include <stdlib.h>

#include "shed.h"

struct shed {
	int limit;
	long window;   /* 지금 구간 번호 (now_ms / 1000) */
	int count;     /* 이 구간에서 받은 수 */
	int started;
	long admitted;
	long rejected;
};

shed_t *shed_create(int tps_limit)
{
	shed_t *s;

	if (tps_limit < 1)
		return NULL;
	s = calloc(1, sizeof(*s));
	if (s != NULL)
		s->limit = tps_limit;
	return s;
}

int shed_admit(shed_t *s, long now_ms)
{
	long w;

	if (s == NULL)
		return 0;
	w = now_ms / 1000;
	if (!s->started || w > s->window) {
		s->started = 1;
		s->window = w;
		s->count = 0;
	}
	if (s->count < s->limit) {
		s->count++;
		s->admitted++;
		return 1;
	}
	s->rejected++;
	return 0;
}

void shed_stats(const shed_t *s, long *admitted, long *rejected)
{
	if (s == NULL)
		return;
	if (admitted)
		*admitted = s->admitted;
	if (rejected)
		*rejected = s->rejected;
}

void shed_destroy(shed_t *s)
{
	free(s);
}

int shed_cut_percent(int cpu_pct)
{
	if (cpu_pct < 50)
		return 0;
	if (cpu_pct < 70)
		return 20;
	if (cpu_pct < 90)
		return 50;
	return 90;
}

int shed_pick_reject(int cut_percent, unsigned seq)
{
	if (cut_percent <= 0)
		return 0;
	if (cut_percent >= 100)
		return 1;
	return (int)(seq % 100) < cut_percent;
}

long backoff_ms(int attempt, long base_ms, long cap_ms, int jitter_pct, unsigned rnd)
{
	long d, lo;
	int i;

	if (base_ms < 1)
		return 0;
	if (attempt < 0)
		attempt = 0;
	if (jitter_pct < 0)
		jitter_pct = 0;
	if (jitter_pct > 100)
		jitter_pct = 100;
	d = base_ms < cap_ms ? base_ms : cap_ms;
	for (i = 0; i < attempt && d < cap_ms; i++) {
		d = (d > cap_ms / 2) ? cap_ms : d * 2;
	}
	lo = d * (100 - jitter_pct) / 100;
	return lo + (d - lo) * (long)(rnd % 1001) / 1000;
}
