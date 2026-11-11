#include <errno.h>
#include <stddef.h>

/* P014 run_step — 요구사항은 problem.md */
int run_step(int (*step)(void *), void (*cleanup)(void *), void *ctx)
{
	(void)step;
	(void)cleanup;
	(void)ctx;
	/* TODO */
	return -1;
}
