#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* P009 rec_create — 요구사항은 problem.md */
typedef struct {
	char *name;     /* 이름 복사본 (힙) */
	int *vals;      /* nvals 개, 0 으로 초기화 (힙) */
	size_t nvals;
} rec_t;

int rec_create(rec_t **out, const char *name, size_t nvals)
{
	(void)out;
	(void)name;
	(void)nvals;
	/* TODO */
	return -1;
}

void rec_destroy(rec_t *r)
{
	(void)r;
	/* TODO */
}
