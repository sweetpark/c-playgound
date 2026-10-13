#include <stdio.h>
#include <stdlib.h>

enum { MIN_SIZE_ALLOWED = 32 };

int verify_size(char *list, size_t size) {
	if (size < MIN_SIZE_ALLOWED) {
		printf("size(%d) is too short\n", size) ;
		//free(list);
		return -1;
	}
	return 0;
}

void process_list(size_t number) {
	char *list = (char *)malloc(number);
	if (list == NULL) {
		printf("malloc fail\n") ;
		return ;
	}

	if (verify_size(list, number) == -1) {
		// 2번 free
		free(list);
		return;
	}

	/* process code */

	free(list);
}

int main(void)
{
	process_list( 100 ) ;

	return 0 ;
}
