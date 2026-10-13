#include <stdio.h>
#include <string.h>

int main(void)
{
	size_t len;
	char cstr[] = "char string";
	signed char scstr[] = "signed char string";
	unsigned char ucstr[] = "unsigned char string";

	len = strlen(cstr);
	len = strlen(scstr);
	len = strlen(ucstr);

	return 0;
}
