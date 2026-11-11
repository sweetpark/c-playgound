#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* P012 sbuf_append — 요구사항은 problem.md */
#define SBUF_MAX 64   /* 널 문자 포함 최대 바이트 */

typedef struct {
	size_t len;       /* 현재 문자열 길이 (널 제외) */
	size_t cap;       /* data 에 할당된 바이트 */
	char data[];
} sbuf_t;

/* cap 바이트짜리 빈 문자열 버퍼. cap == 0 또는 > SBUF_MAX 이면 NULL */
sbuf_t *sbuf_new(size_t cap)
{
	(void)cap;
	/* TODO */
	return NULL;
}

/* *ps 의 끝에 s 를 붙인다. 모자라면 realloc 으로 늘린다(주소가 바뀔 수 있다).
 * 0 성공 / -1 실패(인자 오류, SBUF_MAX 초과, realloc 실패) — 실패하면 *ps 는 원래대로 유효 */
int sbuf_append(sbuf_t **ps, const char *s)
{
	(void)ps;
	(void)s;
	/* TODO */
	return -1;
}
