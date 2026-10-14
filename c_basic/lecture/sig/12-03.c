#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
enum { MAXLINE = 1024 };
char *info = NULL;

/* log_message에서 문제가 발생시 lock이 걸릴 수 있음 */
void log_message(void) {
	fputs(info, stderr);
}

void handler(int signum) {
	log_message();
	free(info);
	info = NULL;
}

int main(void) {
	int cnt = 1 ;
	if (signal(SIGINT, handler) == SIG_ERR) {
		printf("signal(SIGINT) fail\n") ;
		return 1 ;
	}

	info = (char *)malloc(MAXLINE);
	if (info == NULL) {
		printf("malloc fail\n");
		return 1 ;
	}

	while (1) {
		if( info == NULL ) break ;
		
		snprintf(info, MAXLINE, "cnt:%d\n", cnt) ;

		log_message();

		cnt++ ;

		sleep(1) ;
	}
	return 0;
}

