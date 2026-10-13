#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 구조체 선언 */
typedef struct array{
	int size;		/* str_chr에 할당된 크기(capacity) */
	char str_chr[];
} Array;


#define MAX_SIZE 20

/* 현재 저장된 문자열 길이 (널 문자 전까지) */
int getDstStrlen(Array *list){

	int count = 0;

	while( count < list->size && list->str_chr[count] != '\0' ){
		count++;
	}

	return count;
}

/* realloc으로 list 주소가 바뀔 수 있으므로 이중 포인터로 받는다 */
int append(Array **plist, const char *src){

	Array *list = *plist;

	/* 기존 dst len 값 추출 */
	int dst_len = getDstStrlen(list);
	int src_len = (int)strlen(src);
	int need = dst_len + src_len + 1;	/* 널 문자 포함 필요 크기 */

	/* 기존 값 + src 값 합쳤을 때 최대 크기를 벗어나는지 체크 */
	if( need > MAX_SIZE ){
		printf("MAX size over\n");
		return -1;
	}

	/* 기존 값 + src 값이 현재 size를 벗어날경우 필요한 크기만큼 realloc 해주기 */
	if( list->size < need ){
		Array *tmp = (Array *)realloc(list, sizeof(Array) + need);
		if( tmp == NULL ){
			printf("realloc failed\n");
			return -1;		/* 기존 list는 그대로 유효 */
		}
		list = tmp;
		list->size = need;
		*plist = list;
	}

	/* 값 append 하기 (널 문자까지 복사) */
	memcpy(&list->str_chr[dst_len], src, src_len + 1);

	return 0;
}


int main(void){

	int size = 10;
	Array* list = (Array *)malloc(sizeof(Array) + sizeof(char) * size);

	char * temp_str1 = "test";
	char * temp_str2 = "01234567890123456789";

	if(list == NULL){
		printf("malloc failed\n");
		return -1;
	}

	list->size = size;
	list->str_chr[0] = '\0';	/* 빈 문자열로 초기화 */

	/* append() */
	append(&list, temp_str1);
	printf("%s\n", list->str_chr);

	/* 20자 + 기존 4자 > MAX_SIZE 이므로 실패해야 함 */
	append(&list, temp_str2);
	printf("%s\n", list->str_chr);

	/* 정리 */
	if(list != NULL) {
		free(list);
	}


	return 0;
}
