#include <stdio.h>
#include <stdlib.h>

struct flex_array_struct {
	size_t num;
	int data[];
};

void print_array(struct flex_array_struct struct_p) {
	puts("Array is: ");

	size_t i ;
	for (i = 0; i < struct_p.num; ++i) {
		printf("%d ", struct_p.data[i]);
	}
	putchar('\n');
}


void func(void) {
	struct flex_array_struct *fas_a;
	size_t array_size = 4;

	fas_a = (struct flex_array_struct *)malloc( sizeof(struct flex_array_struct) + sizeof(int)*array_size ) ;
	if(fas_a == NULL ) {
		return ;
	}

	fas_a->num = array_size ;

	int cnt ;

	for( cnt = 0; cnt < array_size; cnt++ ) {
		fas_a->data[cnt] = cnt+1 ;
	}

	print_array(*fas_a);
}

int main(void)
{
	func() ;

	return 0 ;
}
