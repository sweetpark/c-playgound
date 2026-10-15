#include <pthread.h>
#include <stdio.h>
 
void *do_work(void *arg){

	printf("[%u] start\n", pthread_self() ) ;

	/* detach로 해당 스레드 종료 */
	if( pthread_detach( pthread_self() ) != 0 ) {
		printf("pthread_detach fail\n") ;
	}

	return NULL;
}

int main(void){
	pthread_t thr;

	if (pthread_create(&thr, NULL, do_work, NULL) != 0) {
		fprintf(stderr, "Creation of thread failed\n");
		return 1 ;
	}

	sleep(1) ; 

	/*이미 종료된 thr은 실패 처리됨*/
	if (pthread_join(thr, NULL) != 0) {
		fprintf(stderr, "pthread_join failed\n");
		return 1 ;
	}

	return 0;
}
