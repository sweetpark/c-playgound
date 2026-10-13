#include <stdio.h>

int prt_file_content( FILE *fp) {
	if( fp == NULL ) return -1 ;

	int ch ;

	while( (ch = fgetc(fp)) != EOF ) {
		putchar(ch) ;
	}

	//fseek() 함수 사용 지향 (rewind 는 성공실패 여부 반환값 없음)
	rewind(fp) ;

	return 1 ;
}

int main(void) {
	FILE *fp ;

	fp = fopen("./10-03.txt", "r") ;
	if( fp == NULL ) {
		printf("fopen fail\n") ;
		return 1 ;
	}

	if( prt_file_content(fp) < 0 ) {
		printf("prt_file_content() fail\n") ;

		return 1 ;
	}

	/* process file */

	//flcose 누락
	return 1 ;
}
