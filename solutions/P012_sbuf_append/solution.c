#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* P012 sbuf_append — 모범답안 */
#define SBUF_MAX 64

typedef struct {
	size_t len;
	size_t cap;
	char data[];
} sbuf_t;

sbuf_t *sbuf_new(size_t cap)
{
	sbuf_t *b;

	if (cap == 0 || cap > SBUF_MAX)
		return NULL;
	b = malloc(sizeof(*b) + cap);
	if (b == NULL)
		return NULL;
	b->len = 0;
	b->cap = cap;
	b->data[0] = '\0';
	return b;
}

int sbuf_append(sbuf_t **ps, const char *s)
{
	sbuf_t *b, *tmp;
	size_t slen, need;

	if (ps == NULL || *ps == NULL || s == NULL)
		return -1;
	b = *ps;
	slen = strlen(s);
	need = b->len + slen + 1;              /* 널 문자 포함 */
	if (need > SBUF_MAX)
		return -1;

	if (need > b->cap) {
		tmp = realloc(b, sizeof(*b) + need);   /* 바로 b 에 받으면 실패 시 원본을 잃는다 */
		if (tmp == NULL)
			return -1;                          /* b 는 그대로 유효 */
		tmp->cap = need;
		b = tmp;
		*ps = b;                                /* 호출자 포인터 갱신 */
	}
	memcpy(b->data + b->len, s, slen + 1);      /* 위치는 cap 이 아니라 len */
	b->len += slen;
	return 0;
}
