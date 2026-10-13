#include <stdio.h>

struct flex_array_struct {
	size_t num ;
	int data[] ;
} ;

void func(void) {
	struct flex_array_struct flex_struct ;
	size_t array_size = 4 ;

	flex_struct.num = array_size ;

	printf("flex_struct address:%p(size:%zu)\n", &flex_struct, sizeof(struct flex_array_struct) ) ;

	size_t i ;
	for( i = 0; i < array_size; i++ ) {
		printf("flex_struct.data[%d] address: %p\n", i, &flex_struct.data[i] ) ;
		flex_struct.data[i] = 0 ;
	}

}

int main(void) {
	func() ;

	return 0 ;
}
