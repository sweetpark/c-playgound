/* P108 ctr 모범답안 */
#include <stdlib.h>

#include "ctr.h"

struct ctr {
	unsigned long long max;
	unsigned long long last;
	int has_last;
};

ctr_t *ctr_create(unsigned long long max)
{
	ctr_t *c;

	if (max < 3)
		return NULL;
	c = calloc(1, sizeof(*c));
	if (c != NULL)
		c->max = max;
	return c;
}

ctr_kind_t ctr_feed(ctr_t *c, unsigned long long value, unsigned long long *delta)
{
	ctr_kind_t kind;
	unsigned long long d;

	if (c == NULL || delta == NULL)
		return CTR_FIRST;
	if (!c->has_last) {
		kind = CTR_FIRST;
		d = 0;
	} else if (value >= c->last) {
		kind = CTR_OK;
		d = value - c->last;
	} else if (c->last > c->max - c->max / 4) {
		kind = CTR_WRAP;
		d = (c->max - c->last) + 1 + value;
	} else {
		kind = CTR_RESET;
		d = value;
	}
	c->last = value;
	c->has_last = 1;
	*delta = d;
	return kind;
}

void ctr_destroy(ctr_t *c)
{
	free(c);
}

double wavg(const double *avg, const unsigned long *cnt, int n)
{
	double sum = 0.0;
	unsigned long total = 0;
	int i;

	if (avg == NULL || cnt == NULL || n <= 0)
		return 0.0;
	for (i = 0; i < n; i++) {
		sum += avg[i] * (double)cnt[i];
		total += cnt[i];
	}
	return total == 0 ? 0.0 : sum / (double)total;
}

int sum_check(unsigned long req, unsigned long rsp, unsigned long timeout, unsigned long cancel,
              unsigned long inflight, long *diff)
{
	long d = (long)req - (long)(rsp + timeout + cancel + inflight);

	if (diff != NULL)
		*diff = d;
	return d == 0;
}
