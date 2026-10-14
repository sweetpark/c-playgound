#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {PATH_MAX=32};

void f()
{
	char path[PATH_MAX] ;

	/* 환경변수의 변화에 따라 달라질 수 있다. 값 길이가 넘칠 수 있다 */
	strcpy(path, getenv("PATH") ) ;
	/*strncpy(path, getenv("PATH"),32) ;*/
	/*path[PATH_MAX-1] = '\0';*/
	printf("path=%s\n", path) ;
}

int main(void)
{
	f() ;

	return 0 ;
}
