#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

void func(const char *c_str)
{
	unsigned long number ;
	char *endptr ;

	number = strtoul(c_str, &endptr, 0) ;
	if( endptr == c_str || (number == ULONG_MAX && errno == ERANGE) )
	{
		printf("%s is invalid\n", c_str ) ;
		return ;
	}

	printf("number is %lu\n", number) ;
}

int main(void)
{
	const char *ulong_max="0xffffffffffffffff";
	const char *ulong_max_exceed="0x10000000000000000";

	func(ulong_max) ;

	func(ulong_max_exceed) ;

	func("0x100") ;

	return 0 ;
}

