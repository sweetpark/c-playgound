#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
 
const size_t thread_no = 5;
const char mess[] = "This is a test";

void *message_print(void *ptr){
	const char *msg = (const char *) ptr;

	printf("THREAD[%u]: start \n", pthread_self());

	sleep( 1 ) ;

	printf("THREAD[%u]: This is the Message %s\n", pthread_self(), msg);

	return NULL;
}

int main(void){
	pthread_t thr[thread_no];
	size_t i ;

	for (i = 0; i < thread_no; ++i) {
		if (pthread_create(&(thr[i]), NULL, message_print,(void *)mess) != 0) {
			fprintf(stderr, "Creation of thread %zu failed\n", i);
			return 1 ;
		}
	}

	printf("MAIN: Thread Message: %s\n", mess);
#if 1
	for (i = 0; i < thread_no; ++i){
		pthread_join(thr[i]);
		//pthread_detach : 다른스레드는 기다리지 않고 해당 스레드 종료
	}
#endif 
	return 0;
}
