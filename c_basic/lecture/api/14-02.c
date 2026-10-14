#include <stdio.h>

int copy(char *dst, const char *src) {
	if( dst == NULL || src == NULL ) {
		return -1 ;
	}

	while( *src != '\0' ) {
		*dst++ = *src++ ;
	}
	*dst = '\0' ;
}

int main(void)
{
	const char *src = "Hello! James" ;

	char dst[100] ;

	if( copy(dst, src) < 0 ) {
		printf("copy fail\n") ;
		return 1 ;
	}

	printf("dst=%s\n", dst) ;

	return 0 ;
}
