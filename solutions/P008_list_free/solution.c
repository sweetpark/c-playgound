#include <stddef.h>
#include <stdlib.h>

/* P008 list_free — 모범답안 */
typedef struct node {
	int val;
	struct node *next;
} node_t;

int list_push(node_t **head, int v)
{
	node_t *n;

	if (head == NULL)
		return -1;
	n = malloc(sizeof(*n));
	if (n == NULL)
		return -1;
	n->val = v;
	n->next = *head;
	*head = n;
	return 0;
}

int list_remove(node_t **head, int v)
{
	node_t **pp;

	if (head == NULL)
		return 0;
	for (pp = head; *pp != NULL; pp = &(*pp)->next) {
		if ((*pp)->val == v) {
			node_t *victim = *pp;
			*pp = victim->next;      /* 앞 노드(또는 head)가 다음 노드를 가리키게 */
			free(victim);
			return 1;
		}
	}
	return 0;
}

size_t list_free(node_t **head)
{
	node_t *cur, *next;
	size_t count = 0;

	if (head == NULL)
		return 0;
	for (cur = *head; cur != NULL; cur = next) {
		next = cur->next;            /* free 전에 다음 노드를 저장 */
		free(cur);
		count++;
	}
	*head = NULL;
	return count;
}
