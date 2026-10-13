#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { BUFFER_SIZE = 32 } ;

int f() {
	char *text_buffer = (char *)malloc(BUFFER_SIZE) ;
	if(text_buffer == NULL) {
		return -1 ;
	}

	FILE *fp ;

	fp = fopen("9-09.txt", "r") ;
	if( fp == NULL ) {
		printf("fopen fail\n") ;
		return -1 ;
	}

	if( fgets(text_buffer, BUFFER_SIZE, fp) == NULL ) {
		printf("fgets fail\n");
		fclose(fp) ;
		/* free 누락 */
		/* free(text_buffer); */
		return -1 ;
	}

	if( strlen(text_buffer) == (BUFFER_SIZE-1) )
	{
		if( text_buffer[BUFFER_SIZE-1] == '\n' ) {
			text_buffer[BUFFER_SIZE-1] = '\0' ;
		}
	}
	else {
		text_buffer[strlen(text_buffer)-1] = '\0' ;
	}

	printf("1st line:%s", text_buffer ) ;

	fclose(fp) ;
	/* free 누락 */
	/* free(text_buffer); */
	
	return 1 ;
}

int main(void)
{
	if( f() < 0 ) {
		printf("f() fail\n") ;
		return 1 ;
	}

	return 0 ;
}
