#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(void) {
	/* errno 가 printf 지나갈때마다 초기화되므로 정상적 체크가 안됨 >> 한번에 처리할경우 출력 fp의 ferror(stdout) 처리해야함 */
	errno = 0 ;
	printf("This\n") ;
	printf("is\n") ;
	printf("a\n") ;
	printf("test.\n") ;

	if( errno != 0 ) {
		fprintf(stderr, "printf failed: %s\n", strerror(errno) ) ;
		return 1 ;
	}

	return 0 ;
}
