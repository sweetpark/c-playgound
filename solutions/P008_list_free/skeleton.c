#include <stddef.h>
#include <stdlib.h>

/* P008 list_free — 요구사항은 problem.md */
typedef struct node {
	int val;
	struct node *next;
} node_t;

/* v 를 가진 새 노드를 맨 앞에 붙인다. 성공 0 / 실패 -1 */
int list_push(node_t **head, int v)
{
	(void)head;
	(void)v;
	/* TODO */
	return -1;
}

/* 값이 v 인 첫 노드를 떼어 해제. 지웠으면 1, 없으면 0 */
int list_remove(node_t **head, int v)
{
	(void)head;
	(void)v;
	/* TODO */
	return 0;
}

/* 모든 노드 해제, *head = NULL. 해제한 개수를 반환 */
size_t list_free(node_t **head)
{
	(void)head;
	/* TODO */
	return 0;
}
