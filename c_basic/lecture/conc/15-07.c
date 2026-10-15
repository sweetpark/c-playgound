#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
 
pthread_mutex_t mutex;
pthread_mutexattr_t attr ;

void *do_work2(void *arg) {
	if( pthread_mutex_unlock(&mutex) != 0 ) 
	{
		printf("pthread_mutex_unlock fail\n") ;
	}

	return NULL ;
}

void *do_work1(void *arg) {
	if( pthread_mutex_lock(&mutex) != 0 )
	{
		printf("pthread_mutex_lock fail\n") ;

		return NULL ;
	}

	sleep(2) ;


	if( pthread_mutex_unlock(&mutex) != 0 ) 
	{
		printf("pthread_mutex_unlock fail\n") ;
	}

	return NULL ;
}

int main(void){
	pthread_t thr[2];
	size_t i ;
	int ret ;

	if (pthread_mutexattr_init( &attr) != 0) {
		printf("pthread_mutexattr_init fail\n") ;
		return 1 ;
	} 

	if( pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_NORMAL) != 0 ) {
		printf("pthread_mutexattr_settype fail\n") ;
		return 1 ;
	}

	if( pthread_mutex_init(&mutex, &attr) != 0 ) {
		printf("pthread_mutex_init fail\n" ) ;
		return 1 ;
	}

#if 0
	printf("unlock unlocked mutex==============>\n" ) ;
	if (pthread_create(&thr[0], NULL, do_work2, NULL) != 0) {
		fprintf(stderr, "Creation of thread failed\n");
		return ;
	}

	if( pthread_join(thr[0], NULL ) != 0 ) {
		printf("pthread_join fail\n") ;
	}
#endif

#if 1
	printf("unlock locked mutex by other thread==============>\n" ) ;
	if (pthread_create(&thr[0], NULL, do_work1, NULL) != 0) {
		fprintf(stderr, "Creation of thread failed\n");
		return ;
	}

	if (pthread_create(&thr[1], NULL, do_work2, NULL) != 0) {
		fprintf(stderr, "Creation of thread failed\n");
		return ;
	}

	for( i = 0 ; i < 2 ; i++ ) {
		if( pthread_join(thr[i], NULL ) != 0 ) {
			printf("pthread_join fail\n") ;
		}
	}
#endif

	if( ( ret = pthread_mutex_destroy(&mutex) ) != 0 ) {
		printf("pthread_mutex_destroy fail, ret=%d(%s)\n", ret, strerror(ret) ) ;
		return 1 ;
	}

	return 0;
}
