#include <stdio.h>
#include <string.h>

FILE *fp ;
enum { BUFFER_SIZE = 1024 };

void func(void) {
	char buf[BUFFER_SIZE];

	if (fgets(buf, sizeof(buf), fp) == NULL) {
		printf("fgets error\n") ;
		return ;
	}

	printf("read length:%d\n", strlen(buf) ) ;

	// byte로 열었기 때문에, 해당 '\0'을 하면 안됨
	buf[strlen(buf) - 1] = '\0';
}

int main(void)
{
	fp = fopen("./10-04-01.dat", "rb") ;
	if( fp == NULL )
	{
		printf("fopen fail\n") ;
		return -1;
	}

	func();

	fclose(fp);

	return 0;
}

