#include <stdio.h>
#include <stdlib.h>

struct flex_array_struct {
	size_t num;
	int data[];
};

int copy(struct flex_array_struct *struct_a, struct flex_array_struct *struct_b) {
	
	int i = 0;

	if( struct_a == NULL || struct_b == NULL ) {
		return -1 ;
	}

	// 처음 num만 복사됨
	*struct_b = *struct_a;
/*
	for(i = 0; i < struct_a->num; i++){

		struct_b->data[i] = struct_a->data[i];
	}
*/	
	return 1 ;
}

int main(void)
{
	struct flex_array_struct *fas_a = NULL, *fas_b = NULL ;

	fas_a = (struct flex_array_struct *)malloc( sizeof(struct flex_array_struct) + sizeof(int)*4 ) ;
	if(fas_a == NULL ) {
		return 1 ;
	}
	
	fas_a->num = 4 ;

	int cnt ;

	for( cnt = 0; cnt < 4; cnt++ ) {
		fas_a->data[cnt] = cnt+1 ;
	}

	fas_b = (struct flex_array_struct *)malloc( sizeof(struct flex_array_struct) + sizeof(int)*4 ) ;
	if(fas_b == NULL ) {
		return 1 ;
	}
	
	if( copy(fas_a, fas_b) < 0 ) {
		return 1 ;
	}

	printf("fas_b->num:%d\n", fas_b->num ) ;
	for( cnt = 0; cnt < 4; cnt++ ) {
		printf("fas_b->data[%d] = %d\n", cnt, fas_b->data[cnt] ) ;
	}

	/* free 누락 */
	/*
	 * free(fas_a);
	 * free(fas_b);
	 */
	return 0 ;
}
