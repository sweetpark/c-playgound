#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* P011 blob_clone — 모범답안 */
typedef struct {
	size_t n;
	int data[];
} blob_t;

blob_t *blob_new(size_t n)
{
	blob_t *b;

	if (n == 0 || n > 1024)
		return NULL;
	b = malloc(sizeof(*b) + n * sizeof(b->data[0]));   /* 헤더 + 데이터 */
	if (b == NULL)
		return NULL;
	b->n = n;
	memset(b->data, 0, n * sizeof(b->data[0]));
	return b;
}

blob_t *blob_clone(const blob_t *src)
{
	size_t bytes;
	blob_t *dup;

	if (src == NULL)
		return NULL;
	bytes = sizeof(*src) + src->n * sizeof(src->data[0]);  /* *dup = *src 는 헤더(8바이트)만 복사 */
	dup = malloc(bytes);
	if (dup == NULL)
		return NULL;
	memcpy(dup, src, bytes);
	return dup;
}

long blob_sum(const blob_t *b)
{
	long s = 0;
	size_t i;

	if (b == NULL)
		return 0;
	for (i = 0; i < b->n; i++)
		s += b->data[i];
	return s;
}
