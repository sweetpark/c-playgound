#include <stdio.h>
#include <stdlib.h>

struct node {
	int value ;
	struct node *next ;
} ;

void free_list(struct node *head) {
	struct node *p ;

	if( head == NULL ) return ;

	// p가 해제된 이후 p->next 참조 오류
	for( p = head ; p != NULL ; p = p->next ) {
		free(p) ;
	}
}

struct node *init_list() {
	struct node *temp ;

	temp = (struct node *)malloc(sizeof(struct node)) ;
	if( temp == NULL )
		return NULL ;

	temp->value = 0 ;
	temp->next = NULL ;

	return temp ;
} 

int insert_list(struct node *head, int value) 
{
	struct node *temp ;

	if( head == NULL ) return -1 ;

	temp = (struct node *)malloc(sizeof(struct node)) ;
	if( temp == NULL ) {
		return -1 ;
	}

	temp->value = value ;
	temp->next = head->next ;
	head->next = temp ;

	return 1 ;
}

void prt_list(struct node *head) {
	struct node *p ;

	if( head == NULL ) return ;

	p = head->next ;

	// while loop 종료문
	while( p ) {
		printf("value:%d\n", p->value) ;
		p = p->next ;
	}
}

int main()
{
	int a[5] = {3, 1, 5, 4, 2} ;

	struct node *head = init_list() ;
	if( head == NULL ) return 1 ;

	int cnt ;
	for( cnt = 0; cnt < 5; cnt++ ) {
		if( insert_list(head, a[cnt]) < 0 ) {
			return 1 ;
		}
	} 

	prt_list(head) ;

	free_list(head) ;

	return 0 ;
}
