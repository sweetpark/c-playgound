#include <stdio.h>
#include <string.h>

void func(const char *src)
{
	char a[16] ;

	/*
	 *
	 * src NULL 체크
	 * src size 계산 (a보다 작게)
	 *
	 * */
	strncpy(a, src, sizeof(a)) ;

	printf("a=%s\n", a) ;

}

int main(void)
{
	char b[] = "123456789";

	func(b) ;

	return 0 ;
}
