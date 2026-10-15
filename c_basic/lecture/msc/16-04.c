#include <stdio.h>
#include <stdint.h>

void f(size_t begin, size_t step) {
	size_t i;
	printf("start loop\n") ;
	for (i = begin; i <= SIZE_MAX; i += step) {
		printf("i=%u\n", i) ;
	}
	printf("end loop\n") ;
}

int main(void)
{
	f(SIZE_MAX-10, 12) ;
	return 0 ;
}
