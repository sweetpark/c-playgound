#include <stdio.h>
#include <stddef.h>

void copy(size_t n, char *src, char *dest) {
	size_t i;

	for (i = 0; src[i] && (i < n); ++i) {
		dest[i] = src[i];
	}
	printf("%d\n", i);
	// overflow
	dest[i] = '\0';
}

int main(void)
{
	char src[10] = "0123456789";
	char dest[10] ;

	copy(10, src, dest) ;

	printf("dest=%s\n", dest) ;

	return 0;
}
