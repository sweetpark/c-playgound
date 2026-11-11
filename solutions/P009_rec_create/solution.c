#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* P009 rec_create — 모범답안 (단일 출구 정리: goto fail) */
typedef struct {
	char *name;
	int *vals;
	size_t nvals;
} rec_t;

int rec_create(rec_t **out, const char *name, size_t nvals)
{
	rec_t *r = NULL;
	char *nm = NULL;
	int *v = NULL;
	size_t len;

	if (out == NULL || name == NULL || nvals == 0 || nvals > 1024)
		return -1;
	len = strlen(name);

	r = malloc(sizeof(*r));                 /* 1 */
	if (r == NULL) goto fail;
	nm = malloc(len + 1);                   /* 2 */
	if (nm == NULL) goto fail;
	v = malloc(nvals * sizeof(*v));         /* 3 */
	if (v == NULL) goto fail;

	memcpy(nm, name, len + 1);
	memset(v, 0, nvals * sizeof(*v));
	r->name = nm;
	r->vals = v;
	r->nvals = nvals;
	*out = r;                               /* 성공했을 때만 호출자에게 */
	return 0;

fail:
	free(v);                                /* 획득의 역순, free(NULL) 은 안전 */
	free(nm);
	free(r);
	return -1;
}

void rec_destroy(rec_t *r)
{
	if (r == NULL)
		return;
	free(r->vals);
	free(r->name);
	free(r);
}
