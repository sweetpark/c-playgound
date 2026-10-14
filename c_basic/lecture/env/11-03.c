#include <stdlib.h>
#include <stdio.h>

void exit1(void) {
	printf("exit1\n") ;
	return;
}

void exit2(void) {
	printf("exit2\n") ;
	return;
}

int main(int argc, char *argv[]) {
	if (atexit(exit1) != 0) {
		printf("atexit(exit1) fail\n") ;
		return -1 ;
	}
	if (atexit(exit2) != 0) {
		printf("atexit(exit2) fail\n") ;
		return -1 ;
	}

	if( argc != 1 ) {
		printf("Usage: %s\n", argv[0] ) ;
		_Exit(1) ;
	}

	printf("before return\n") ;

	return 0;
}
