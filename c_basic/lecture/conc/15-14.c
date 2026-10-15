#include <pthread.h>
#include <stdio.h>
#include <signal.h>

#if 1
volatile int flag = 0;

void stop_flg(int signo){
	flag = 1;
}

#endif
void *func(void *foo) {
#if 1
	signal(SIGTERM, stop_flg);
#endif
	/* signal 처리가 되지 않아 전체 종료가 되버림  */
	while(!flag) {
		printf("alive...thread\n") ;
		sleep(1) ;
	}

	printf("pthread end\n") ;

	return NULL ;
}
 
int main(void) {
	int result;
	pthread_t thread;

	if ((result = pthread_create(&thread, NULL, func, 0)) != 0) {
		printf("pthread_create fail\n") ;
		return 1 ;
	}

	sleep(1) ;

	if ((result = pthread_kill(thread, SIGTERM)) != 0) {
		printf("pthread_kill fail\n") ;
		return 1 ;
	}

	printf("main thread end\n") ;

	return 0;
}
