#include <stdio.h>
#include <string.h>
#include <errno.h>

FILE *create_file(const char *file_name)
{
	FILE *fp ;

	if( file_name == NULL ) return NULL ;

	/*
	 * 이미 존재하는 파일 필터링하는 로직 존재 x
	fp = fopen(file_name, "w") ;
	if( !fp )
	{
		printf("fopen(%s) fail[%s]\n", file_name, strerror(errno) ) ;
		return NULL ;
	}
	*/
	
	/* lstat 의 경우 원자적처리가 되지 않아서 지양
	 * open()을 사용하여, 옵션을줘서 원자적처리로 안전하게 파일 열기
	*/
	
	fp = fopen(file_name, "wx");
	if(fp == NULL && errno == EEXIST){
		printf("already file create\n");
		return NULL;
	}else if (fp == NULL && errno != EEXIST){
		printf("error file create\n");
		return NULL;
	}

	return fp ;
}

int main(void) {
	FILE *fp ;

	fp = create_file("./10-01.txt") ;
	if( fp == NULL ) {
		printf("create_file fail\n" ) ;
		return 1 ;
	}

	printf("create file success\n") ;

	fclose(fp) ;

	return 0 ;
}
