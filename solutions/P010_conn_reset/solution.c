#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* P010 conn_reset — 모범답안 */
typedef struct {
	int fd;
	char host[32];
	unsigned short port;
	struct { long rx, tx; } stats;
	char tag[16];
} conn_t;

void conn_reset(conn_t *c)
{
	if (c == NULL)
		return;
	memset(c, 0, sizeof(*c));     /* sizeof(c) 는 포인터 크기(8) */
	c->fd = -1;
}

void conn_reset_all(conn_t *arr, size_t n)
{
	size_t i;

	if (arr == NULL)
		return;
	for (i = 0; i < n; i++)
		conn_reset(&arr[i]);
}

int conn_init(conn_t *c, const char *host, unsigned short port)
{
	if (c == NULL || host == NULL)
		return -1;
	conn_reset(c);
	snprintf(c->host, sizeof(c->host), "%s", host);   /* 잘라서, 항상 널 종료 */
	c->port = port;
	return 0;
}
