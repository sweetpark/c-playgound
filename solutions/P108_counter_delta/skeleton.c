/* P108 ctr — 요구사항은 problem.md. ctr.h 의 선언을 보고 채운다. */
#include <stdlib.h>

#include "ctr.h"

struct ctr {
	/* TODO: 최대값, 이전 값, 이전 값이 있는지 */
	int unused;
};

ctr_t *ctr_create(unsigned long long max)
{
	(void)max;
	/* TODO */
	return NULL;
}

ctr_kind_t ctr_feed(ctr_t *c, unsigned long long value, unsigned long long *delta)
{
	(void)c;
	(void)value;
	(void)delta;
	/* TODO */
	return CTR_FIRST;
}

void ctr_destroy(ctr_t *c)
{
	(void)c;
	/* TODO */
}

double wavg(const double *avg, const unsigned long *cnt, int n)
{
	(void)avg;
	(void)cnt;
	(void)n;
	/* TODO */
	return -1.0;
}

int sum_check(unsigned long req, unsigned long rsp, unsigned long timeout, unsigned long cancel,
              unsigned long inflight, long *diff)
{
	(void)req;
	(void)rsp;
	(void)timeout;
	(void)cancel;
	(void)inflight;
	(void)diff;
	/* TODO */
	return -1;
}
