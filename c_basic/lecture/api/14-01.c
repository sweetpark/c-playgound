#include <stdio.h>

enum {MAX_BUF_SIZE=512} ;

static FILE *myFile = NULL;

void setfile(FILE *file) {
	myFile = file;
}
 
void usefile(void) {
	char buf[MAX_BUF_SIZE];

	while( fgets(buf, MAX_BUF_SIZE, myFile) ) {
		printf("%s", buf) ;
	}
}

int main(void) {
	FILE *fp ;

	fp = fopen("./14-01.txt", "r") ;
	if( fp == NULL ) {
		printf("fopen fail\n") ;
		return 1 ;
	}

	setfile(fp) ;

	usefile() ;

	fclose(fp) ;
	return 0;
}
