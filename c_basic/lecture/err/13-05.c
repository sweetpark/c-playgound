#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int si ;

	if( argc != 2 )
	{
		printf("usage: %s number\n", argv[0] ) ;
		return -1 ;
	}

	/* atoi는 에러를 주지않는다 -> strtol(*ptr, NULL, size) */
	si = atoi(argv[1]) ;

	printf("si=%d\n", si) ;

	return 0 ;
}
