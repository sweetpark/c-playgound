#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER ;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER ;

void rand_sleep()
{
	int seed = time(NULL) ;
	srand(seed);

	int r = rand_r(&seed) ;
	usleep( r % 100 ) ;
}

void *run_step(void *t) {
	static size_t current_step = 0;
	size_t my_step = *(size_t *)t;

	rand_sleep() ;
	if (0 != pthread_mutex_lock(&mutex)) {
		printf("pthread_mutex_lock fail\n") ;
		return NULL ;
	}

	printf("Thread %zu has the lock\n", my_step);
	while (current_step != my_step) {
		printf("Thread %zu is sleeping...\n", my_step);

		if (0 != pthread_cond_wait(&cond, &mutex)) {
			printf("pthread_cond_wait fail\n") ;
			if (0 != pthread_mutex_unlock(&mutex)) {
				printf("pthread_mutex_unlock fail\n") ;
			}
			return NULL ;
		}

		printf("Thread %zu woke up\n", my_step);
	}

	/* Do processing ... */
	printf("Thread %zu is processing...\n", my_step);
	current_step++;

	/* Signal awaiting task */
	if (0 != pthread_cond_signal(&cond)) {
		/* Handle error */
		printf("pthread_cond_signal fail\n") ;
		if (0 != pthread_mutex_unlock(&mutex)) {
			printf("pthread_mutex_unlock fail\n") ;
		}
		return NULL ;
	}

	printf("Thread %zu is exiting...\n", my_step);

	if (0 != pthread_mutex_unlock(&mutex)) {
		printf("pthread_mutex_unlock fail\n") ;
	}
	return NULL;
}

enum {THREAD_NO = 5};
int main(void){
	pthread_t thr[THREAD_NO];
	size_t i ;
	size_t step[THREAD_NO] ;
	int ret = 0 ;

	for( i = 0; i < THREAD_NO ; i++ ) {
		step[i] = i;
		if (pthread_create(&thr[i], NULL, run_step, &step[i] ) != 0) {
			printf("Creation of thread failed\n");
			ret = 1 ;
			goto end ;
		}
	}

	for( i = 0; i < THREAD_NO ; i++ ) {
		if( pthread_join( thr[i], NULL ) != 0 ) {
			printf("pthread_join failed\n");
			ret = 1 ;
		}
	}

end :
	if( pthread_cond_destroy( &cond ) != 0 ) {
		printf("pthread_cond_destroy\n" ) ;
	}

	if( pthread_mutex_destroy( &mutex ) != 0 ) {
		printf("pthread_mutex_destroy\n" ) ;
	}

	return ret;
}
