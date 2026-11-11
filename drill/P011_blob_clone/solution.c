#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* P011 blob_clone — 요구사항은 problem.md */
typedef struct {
	size_t n;       /* data 원소 개수 */
	int data[];     /* 유연 배열 멤버: 헤더 뒤에 이어서 할당 */
} blob_t;

/* 원소 n 개(0 으로 초기화)를 가진 blob. n == 0 또는 n > 1024 이면 NULL */
blob_t *blob_new(size_t n)
{
	(void)n;
	/* TODO */
	return NULL;
}

/* src 의 깊은 복사본 (data 까지). src == NULL 이면 NULL */
blob_t *blob_clone(const blob_t *src)
{
	(void)src;
	/* TODO */
	return NULL;
}

/* data 의 합. b == NULL 이면 0 */
long blob_sum(const blob_t *b)
{
	(void)b;
	/* TODO */
	return -1;
}
