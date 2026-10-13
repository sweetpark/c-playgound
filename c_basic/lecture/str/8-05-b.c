#include <stdio.h>
#include <string.h>

enum { BUFFERSIZE = 10 };

void func(void) 
{
	char buf[BUFFERSIZE];
	char *p;
	int ch;

	p = buf;
	while ((ch = getchar()) != '\n' && ch != EOF) 
	{
		*p++ = (char)ch;
	}

	*p++ = 0;

	if (ch == EOF) 
	{
		printf("error\n");
		return ;
	}

	printf("InputData=%s(%d)\n", buf, strlen(buf)) ;
}


int main(void)
{
	printf("input:=====>\n");
	func() ;

	return 0;
}
