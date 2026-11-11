#include <errno.h>
#include <poll.h>
#include <stddef.h>
#include <unistd.h>

/* P035 write_all — 요구사항은 problem.md */
int write_all(int fd, const void *buf, size_t n)
{
	(void)fd;
	(void)buf;
	(void)n;
	/* TODO */
	return -1;
}
