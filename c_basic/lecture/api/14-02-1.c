#include <stdio.h>

int copy(char *dst, size_t dst_size, const char *src) {
	if( dst == NULL || src == NULL ) {
		printf("ptr NULL\n") ;
		return -1 ;
	} else if (dst_size == 0) {
		printf("dst_size ; 0\n") ;
		return -1 ;
	}

	char *dstend = dst + dst_size ;

	while( *src != '\0' && dst < dstend ) {
		*dst++ = *src++ ;
	}
	*dst = '\0' ;

	return 1 ;
}

int main(void)
{
	const char *src = "Hello! James" ;

	char dst[7] ;

	if( copy(dst, 7, src) < 0 ) {
		printf("copy fail\n") ;
		return 1 ;
	}

	printf("dst=%s\n", dst) ;

	return 0 ;
}
