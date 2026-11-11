#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

#define DIRTY 0xAA

/* 필드를 바이트가 아닌 값으로 검사 (패딩은 보지 않는다) */
static int is_reset(const conn_t *c)
{
	size_t i;
	if (c->fd != -1) return 0;
	for (i = 0; i < sizeof(c->host); i++)
		if (c->host[i] != '\0') return 0;
	if (c->port != 0) return 0;
	if (c->stats.rx != 0 || c->stats.tx != 0) return 0;
	for (i = 0; i < sizeof(c->tag); i++)
		if (c->tag[i] != '\0') return 0;
	return 1;
}

int main(void)
{
	conn_t c;
	conn_t arr[3];
	conn_t *h;
	size_t i;
	int r;
	int ok;

	/* sizeof(conn_t) 는 포인터 크기(8) 보다 훨씬 크다는 전제 */
	CHECK(sizeof(conn_t) > sizeof(void *) * 4);

	/* conn_reset: 쓰레기로 채운 구조체 전체를 비운다 (sizeof(c) 를 쓰면 앞 8바이트만 비워진다) */
	memset(&c, DIRTY, sizeof(c));
	conn_reset(&c);
	CHECK(is_reset(&c));

	/* 힙 구조체: 정확한 크기 → 크기를 넘겨 쓰면 ASan */
	h = malloc(sizeof(*h));
	CHECK(h != NULL);
	if (h) {
		memset(h, DIRTY, sizeof(*h));
		conn_reset(h);
		CHECK(is_reset(h));
		free(h);
	}

	conn_reset(NULL); /* 죽지 않는다 */

	/* conn_reset_all: 배열 전체 */
	for (i = 0; i < 3; i++) memset(&arr[i], DIRTY, sizeof(arr[i]));
	conn_reset_all(arr, 3);
	ok = 1;
	for (i = 0; i < 3; i++)
		if (!is_reset(&arr[i])) ok = 0;
	CHECK(ok);
	conn_reset_all(NULL, 3); /* 죽지 않는다 */
	conn_reset_all(arr, 0);  /* 0 개: 아무 것도 안 한다 */

	/* conn_init: reset + host/port */
	memset(&c, DIRTY, sizeof(c));
	r = conn_init(&c, "example.com", 8080);
	CHECK(r == 0);
	CHECK(c.fd == -1);
	CHECK(memchr(c.host, 0, sizeof(c.host)) && strcmp(c.host, "example.com") == 0);
	CHECK(c.host[sizeof(c.host) - 1] == '\0');
	CHECK(c.host[20] == '\0'); /* 이름 뒤는 모두 0 (이전 쓰레기가 남으면 안 됨) */
	CHECK(c.port == 8080);
	CHECK(c.stats.rx == 0 && c.stats.tx == 0);
	CHECK(c.tag[0] == '\0' && c.tag[sizeof(c.tag) - 1] == '\0');

	/* host 가 버퍼(32)보다 길면 31 글자로 잘리고 널 종료 */
	memset(&c, DIRTY, sizeof(c));
	r = conn_init(&c, "this-hostname-is-definitely-longer-than-thirty-one-chars.example", 1);
	CHECK(r == 0);
	CHECK(memchr(c.host, '\0', sizeof(c.host)) != NULL);
	CHECK(memchr(c.host, 0, sizeof(c.host)) && strlen(c.host) == sizeof(c.host) - 1);
	CHECK(strncmp(c.host, "this-hostname-is-definitely-lon", 31) == 0);
	CHECK(c.port == 1);

	/* 딱 31 글자는 그대로 */
	memset(&c, DIRTY, sizeof(c));
	r = conn_init(&c, "1234567890123456789012345678901", 2);
	CHECK(r == 0);
	CHECK(memchr(c.host, 0, sizeof(c.host)) && strlen(c.host) == 31);

	/* 인자 오류 */
	CHECK(conn_init(NULL, "h", 1) == -1);
	memset(&c, DIRTY, sizeof(c));
	CHECK(conn_init(&c, NULL, 1) == -1);

	return drill_report();
}
