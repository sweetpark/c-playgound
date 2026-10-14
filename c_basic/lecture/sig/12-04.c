#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
 
enum { MAX_MSG_SIZE = 24 };
char *err_msg;
/*free 이후 접근시 잘못된 접근 가능성 존재 */ 
void handler(int signum) {
	strcpy(err_msg, "SIGINT encountered.");
}

int main(void) {
	signal(SIGINT, handler);

	err_msg = (char *)malloc(MAX_MSG_SIZE);
	if (err_msg == NULL) {
		printf("malloc fail\n") ;
		return 1 ;
	}
	sleep(3);
	strcpy(err_msg, "No errors yet.");

	sleep(10) ;

	printf("err_msg:%s\n", err_msg) ;
	free(err_msg) ;

	return 0;
}
