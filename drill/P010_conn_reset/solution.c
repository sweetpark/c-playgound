#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* P010 conn_reset — 요구사항은 problem.md */
typedef struct {
	int fd;                       /* 초기값 -1 (닫힘) */
	char host[32];
	unsigned short port;
	struct { long rx, tx; } stats;
	char tag[16];
} conn_t;

/* 구조체 전체를 0 으로 비우고 fd = -1 */
void conn_reset(conn_t *c)
{
	(void)c;
	/* TODO */
}

/* 배열 n 개를 모두 conn_reset */
void conn_reset_all(conn_t *arr, size_t n)
{
	(void)arr;
	(void)n;
	/* TODO */
}

/* conn_reset 후 host(잘라서)·port 채움. 0 성공 / -1 c 또는 host 가 NULL (c 가 유효하면 건드리지 않음) */
int conn_init(conn_t *c, const char *host, unsigned short port)
{
	(void)c;
	(void)host;
	(void)port;
	/* TODO */
	return -1;
}
