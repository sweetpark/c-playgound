#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

long *alloc_long_array( size_t num_elements ) 
{
	long *buffer = (long *)calloc(num_elements, sizeof(long)) ;
	if( buffer == NULL )
	{
		printf("calloc fail,err=%s\n", strerror(errno)) ;
		return NULL;
	}

	printf("calloc ptr = %p\n", buffer) ;

	return buffer ;
}

int main(void)
{
	long *ptr ;

	ptr = alloc_long_array( 10 ) ;
	if( ptr == NULL )
	{
		printf("alloc_long_array fail\n") ; 
		return 1 ;
	}

	//free 누락
	// free(ptr);
	return 0 ;
}
