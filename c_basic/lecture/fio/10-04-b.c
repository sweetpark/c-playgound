#include <stdio.h>
#include <string.h>

int get_name( char *buf, size_t size ) {
	printf("Input Name:") ;

	// fgets 는 size에 따라 짤리므로 해당 예외처리가 필요
	if( fgets(buf, size, stdin ) ) {
		if( *buf ) {
			buf[strlen(buf)-1] = '\0' ;
		}
	}
	else {
		return -1 ;
	}

	return 1 ;
}

int main(void) {
	char name[10] ;

	if( get_name( name, 10 ) < 0 ) {
		printf("get_name fail\n" ) ;
		return 1 ;
	}

	printf("Inputed Name==>%s\n", name) ;
	return 0 ;
}
