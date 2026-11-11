#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <time.h>

/* P023 wait_signal — 요구사항은 problem.md */
int wait_signal(int signo, int timeout_ms)
{
	(void)signo;
	(void)timeout_ms;
	/* TODO */
	return -1;
}
