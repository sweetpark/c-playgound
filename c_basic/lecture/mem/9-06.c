#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum {MAX_BUF_SIZE = 32} ;

void prt_mem( char *buf, size_t size )
{
	int cnt ;

	if( buf == NULL ) return ;

	for( cnt = 0; cnt < size; cnt++ ) 
	{
		if( (cnt != 0) && (cnt%16 == 0) )
		{
			printf("\n");
		}

		printf("%02x ", (unsigned char)buf[cnt]) ;
	}
	printf("\n") ;
}

void prt_buf_str( char *buf, size_t size )
{
	char *cur_ptr = NULL ;
	int cnt = 0 ;

	if( buf == NULL ) return ;

	cur_ptr = buf ;

	while( (cur_ptr < (buf + size)) && (*cur_ptr != '\0') )
	{
		printf("[%d]%s(len:%d)\n", cnt+1, cur_ptr, strlen(cur_ptr)) ;
		cnt++ ;
		cur_ptr += strlen(cur_ptr) + 1;
	}
} 

void dummy_alloc()
{
	char *buf ;
	int cnt ;
	for( cnt = 0 ; cnt < 1024; cnt++ ) 
	{
		buf = (char *)malloc(102400) ;
		if( buf == NULL )
		{
			printf("malloc fail\n") ;
			return ;
		}

		memset(buf, 0xff, 102400) ;
		free(buf) ;
	}
}

int main(void)
{
	char *buf = NULL ;
	char *ptr = NULL ;
	char *string = "Hello" ;

	dummy_alloc() ;

	buf = (char *)malloc(MAX_BUF_SIZE) ;
	if( buf == NULL )
	{
		printf("malloc fail\n") ;
		return 1 ;
	}

	strcpy(buf, string) ;

	prt_buf_str( buf, MAX_BUF_SIZE ) ;

	prt_mem(buf, 50);
	free(buf) ;

	return 0 ;
}
