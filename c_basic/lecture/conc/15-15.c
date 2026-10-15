#include <pthread.h>
#include <stdio.h>

volatile int a = 5;
volatile int b = 10;

/* Lock to enable threads to access a and b safely */
pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

void* worker_thread(void* dummy) {
	int i;
	int c;
	int result;


	if ((result = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,&i)) != 0) {
		printf("pthread_setcanceltype fail, ret=%d\n", result) ;
		return NULL ;
	}

	/* lock 중간에 cancel 될 수 있음 -> 락 해제가 되지않음*/
	while (1) {
		if ((result = pthread_mutex_lock(&global_lock)) != 0) {
			printf("pthread_mutex_lock fail, ret=%d\n", result) ;
			return NULL ;
		}
		c = b;
		b = a;
		a = c;
		if ((result = pthread_mutex_unlock(&global_lock)) != 0) {
			printf("pthread_mutex_unlock fail, ret=%d\n", result) ;
			return NULL ;
		}
	}
	return NULL;
}


int main(void) {
	int result;
	pthread_t worker;

#if 1
	if ( (result = pthread_mutex_init(&global_lock,NULL) != 0){
		printf("pthread_mutex_init failed, ret=%d \n", result);
		return 1;
	}

#endif

	if ((result = pthread_create( &worker, NULL, worker_thread, NULL)) != 0) {
		printf("pthread_create fail, ret=%d\n", result) ;
		return 1 ;
	}

	sleep(1) ;

	if ((result = pthread_cancel(worker)) != 0) {
		printf("pthread_cancel fail, ret=%d\n", result) ;
		return 1 ;
	}

	if ((result = pthread_join(worker, 0)) != 0) {
		printf("pthread_join fail, ret=%d\n", result) ;
		return 1 ;
	}

	if ((result = pthread_mutex_lock(&global_lock)) != 0) {
		printf("pthread_mutex_lock fail, ret=%d\n", result) ;
		return 1 ;
	}

	printf("a: %i | b: %i\n", a, b);

	if ((result = pthread_mutex_unlock(&global_lock)) != 0) {
		printf("pthread_mutex_unlock fail, ret=%d\n", result) ;
		return 1 ;
	}
	/* mutex destroy 미존재*/
#if 1
	if ( (result = pthread_mutex_destroy(&global_lock)) != 0 ){
		printf("pthread_mutex_destroy() failed, ret=%d\n"), result);
		return 1;
	}

#endif
	return 0;
}
