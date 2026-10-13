#include <stdio.h>
#include <stdlib.h>

int *my_malloc( size_t size )
{
	int *list = (int *)malloc(size) ;
	if( list == NULL ) 
	{
		printf("malloc fail\n") ;
	}
	else
	{
		printf("malloc success\n");
	}

	return list ;
}

int main(void)
{
	int *ptr ;
	
	//size를 0으로 줬을때 문제가 발생
	ptr = my_malloc(sizeof(int)) ;

	if( ptr == NULL )
	{
		printf("my_malloc fail\n") ;
		return -1 ;
	}

	*ptr = 10 ;
	printf("ptr(%p) = %d\n", ptr, *ptr) ; 

	free(ptr) ;
	return 0 ;
}
