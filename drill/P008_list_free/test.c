#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

static size_t len_of(const node_t *h)
{
	size_t n = 0;
	for (; h != NULL; h = h->next) n++;
	return n;
}

/* 리스트 내용이 vals[0..n) 과 같은가 */
static int same(const node_t *h, const int *vals, size_t n)
{
	size_t i;
	for (i = 0; i < n; i++, h = h->next) {
		if (h == NULL || h->val != vals[i]) return 0;
	}
	return h == NULL;
}

int main(void)
{
	node_t *head = NULL;
	int a[] = {3, 2, 1};
	int b[] = {3, 1};
	int c[] = {1};
	int i;

	/* push: 앞에 붙는다 */
	for (i = 1; i <= 3; i++) CHECK(list_push(&head, i) == 0);
	CHECK(len_of(head) == 3);
	CHECK(same(head, a, 3));
	CHECK(list_push(NULL, 9) == -1);

	/* remove: 가운데 */
	CHECK(list_remove(&head, 2) == 1);
	CHECK(same(head, b, 2));
	/* remove: 머리 */
	CHECK(list_remove(&head, 3) == 1);
	CHECK(same(head, c, 1));
	/* remove: 없는 값 */
	CHECK(list_remove(&head, 99) == 0);
	CHECK(same(head, c, 1));
	/* remove: 마지막 하나 → 빈 리스트 */
	CHECK(list_remove(&head, 1) == 1);
	CHECK(head == NULL);
	/* remove: 빈 리스트 / NULL */
	CHECK(list_remove(&head, 1) == 0);
	CHECK(list_remove(NULL, 1) == 0);

	/* 꼬리 노드 제거 */
	for (i = 1; i <= 4; i++) list_push(&head, i); /* 4 3 2 1 */
	CHECK(list_remove(&head, 1) == 1);
	CHECK(len_of(head) == 3);
	CHECK(head && head->next && head->next->next &&
	      head->val == 4 && head->next->val == 3 && head->next->next->val == 2);
	CHECK(head && head->next && head->next->next && head->next->next->next == NULL);

	/* 전체 해제: 해제한 개수 반환, head 는 NULL. free 후 p->next 를 읽으면 ASan 이 잡는다 */
	CHECK(list_free(&head) == 3);
	CHECK(head == NULL);

	for (i = 0; i < 100; i++) list_push(&head, i);
	CHECK(len_of(head) == 100);
	CHECK(list_free(&head) == 100);
	CHECK(head == NULL);

	/* 빈 리스트 / NULL */
	CHECK(list_free(&head) == 0);
	CHECK(list_free(NULL) == 0);

	return drill_report();
}
