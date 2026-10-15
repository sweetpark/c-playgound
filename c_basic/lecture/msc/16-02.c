#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int func(int condition) {
	char *s = NULL;

	if (condition) {
		s = (char *)malloc(10);
		if (s == NULL) {
			printf("malloc fail\n") ;
			return -1 ;
		}

		strcpy( s, "good") ;

		printf("%s\n",s) ;

		free(s) ;

		return 0;
	}
	/* 하위 if문의 경우는 dead code 여서 없애야 함  */
	if (s) {
		printf("???? confusion:%s\n", s) ;
	}

	return 0;
} 

int main(void) {
	if ( func(1) < 0 ) return -1 ;

	if( func(0) < 0 ) return -1 ;

	printf("program success\n") ;

	return 1 ;
}
