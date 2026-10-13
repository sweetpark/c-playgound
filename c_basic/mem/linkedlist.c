#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* node struct (flexible array member 사용) */
typedef struct msg_node{
	struct msg_node *next;
	size_t len;			/* data의 길이 (널 문자 제외) */
	unsigned char data[];		/* 가변 길이 데이터, 항상 '\0'으로 끝남 */
} msg_node_t;

/* list struct: head/tail/count를 관리 (더미 head 노드 없음) */
typedef struct msg_list{
	msg_node_t *head;
	msg_node_t *tail;
	int count;
} msg_list_t;

#define MAX_SIZE 100			/* 리스트에 담을 수 있는 최대 노드 수 */
#define LINE_SIZE 256			/* load 시 한 줄 최대 크기 */

/* init 함수 */
msg_list_t *init(void){
	msg_list_t *list;
	list = (msg_list_t *)malloc(sizeof(msg_list_t));

	if (list == NULL) {
		printf("init failed\n");
		return NULL;
	}
	list->head = NULL;
	list->tail = NULL;
	list->count = 0;

	return list;
}

/* insert 함수: 리스트 맨 뒤에 len 바이트의 src를 복사해 노드 추가 (tail 덕분에 O(1)) */
int insert(msg_list_t *list, const char *src, size_t len){

	msg_node_t *new_ptr = NULL;

	if (list == NULL || src == NULL){
		printf("invalid argument\n");
		return -1;
	}

	if (list->count >= MAX_SIZE){
		printf("list full\n");
		return -1;
	}

	/* 구조체 + 데이터 len + 널 문자 1 만큼만 할당 */
	new_ptr = (msg_node_t *)malloc(sizeof(msg_node_t) + len + 1);
	if (new_ptr == NULL) {
		printf("insert fail\n");
		return -1;
	}

	new_ptr->next = NULL;
	new_ptr->len = len;
	memcpy(new_ptr->data, src, len);
	new_ptr->data[len] = '\0';

	if (list->tail == NULL){	/* 빈 리스트 */
		list->head = new_ptr;
	} else {
		list->tail->next = new_ptr;
	}
	list->tail = new_ptr;
	list->count++;

	return 1;
}

/* index node 삭제 함수 (index는 1부터) */
int delete(msg_list_t *list, int index){

	msg_node_t *pre_node = NULL;
	msg_node_t *target;
	int i;

	if (list == NULL || list->head == NULL){
		printf("list empty\n");
		return -1;
	}
	if (index <= 0 || index > list->count){
		printf("size error\n");
		return -1;
	}

	/* target까지 이동하면서 바로 앞 노드(pre_node)를 기억 */
	target = list->head;
	for (i = 1; i < index; i++){
		pre_node = target;
		target = target->next;
	}

	if (pre_node == NULL){		/* 첫 노드 삭제 */
		list->head = target->next;
	} else {
		pre_node->next = target->next;
	}
	if (target == list->tail){	/* 마지막 노드 삭제 */
		list->tail = pre_node;
	}

	free(target);
	list->count--;

	return 1;
}

/* linked list print 함수 */
void print(const msg_list_t *list){

	msg_node_t *i;
	int count = 1;

	if (list == NULL || list->head == NULL){
		printf("(empty)\n");
		return;
	}

	printf("count=%d\n", list->count);
	for (i = list->head; i != NULL; i = i->next){
		printf("[%p, %d, %zu, %s]\n", (void *)i, count, i->len, i->data);
		count++;
	}
}

/* linked list free 함수: 모든 노드와 list 자체를 해제 */
void free_list(msg_list_t *list){

	msg_node_t *cur;
	msg_node_t *next;

	if (list == NULL){
		return;
	}

	for (cur = list->head; cur != NULL; cur = next){
		next = cur->next;
		free(cur);
	}
	free(list);
}

/* linked list load 함수 (파일 한줄씩 읽어서 msg_node에 삽입), 삽입한 줄 수 반환 */
int load(msg_list_t *list, const char *filename){

	FILE *fp;
	char line[LINE_SIZE];
	int loaded = 0;
	size_t len;

	fp = fopen(filename, "r");
	if (fp == NULL){
		printf("fopen fail\n");
		return -1;
	}

	while (fgets(line, sizeof(line), fp) != NULL){
		len = strlen(line);
		if (len > 0 && line[len - 1] == '\n'){
			line[--len] = '\0';
		}
		if (len > 0 && line[len - 1] == '\r'){	/* CRLF 파일 대비 */
			line[--len] = '\0';
		}

		if (insert(list, line, len) < 0){
			break;
		}
		loaded++;
	}

	fclose(fp);

	return loaded;
}


int main(int argc, char *argv[]){

	msg_list_t *list = init();
	char temp[] = "test";

	if (list == NULL){
		return 1;
	}

	insert(list, temp, strlen(temp));
	insert(list, "second", 6);
	insert(list, "third", 5);
	print(list);

	printf("-- delete(1) (첫 노드)\n");
	delete(list, 1);
	print(list);

	printf("-- delete(9) (없는 index)\n");
	delete(list, 9);

	printf("-- delete(2) (마지막 노드)\n");
	delete(list, 2);
	print(list);

	printf("-- insert after tail delete\n");
	insert(list, "again", 5);
	print(list);

	printf("-- delete all\n");
	delete(list, 1);
	delete(list, 1);
	print(list);

	if (argc > 1){
		printf("-- load(%s)\n", argv[1]);
		printf("loaded %d lines\n", load(list, argv[1]));
		print(list);
	}

	free_list(list);
	return 0;
}
