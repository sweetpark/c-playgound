#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

int write_data(void) {
	const char *filename = "13-03.txt";
	FILE *f = fopen(filename, "w");

	if (f == NULL) {
		printf("fopen() fail\n") ;
		return -1 ;
	}
	fprintf(f, "Hello, World\n");

	abort(); 

	close(f) ;
	return 0;
}

int main(void) {
	write_data();
	return 0;
}

