#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define NOERR		0
typedef struct object {  
	int propertyA, propertyB, propertyC;
} object_t;

int do_something(void){
	FILE *fin1, *fin2;
	object_t *obj;
	int ret_val;

	obj = malloc(sizeof(object_t));
	if (obj == NULL) {
		ret_val = errno;
		return ret_val;  
	}

	fin1 = fopen("some_file", "r");
	if (fin1 == NULL) {
		printf("some_file open fail\n") ;
		free(obj) ;
		// errno 반환 처리
		/*ret_val = errno;
		return ret_val;*/
		return errno;
	}

	fin2 = fopen("some_other_file", "r");
	if (fin2 == NULL) {
		printf("some_other_file open fail\n") ;
		// free 누락
		//free(obj);
		fclose(fin1);
		// errono 반환 처리
		/*ret_val = errno;
		return ret_val;*/
		return errno;
	}

	/* ... More code ... */

	fclose(fin1);
	fclose(fin2);
	free(obj);
	return NOERR;
}

int main(void)
{
	if( do_something() != NOERR ) {
		printf("do_something() fail\n") ;
		return 1 ;
	}

	return 0 ;
}
