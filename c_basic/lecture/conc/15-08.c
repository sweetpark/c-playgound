#include <pthread.h>
#include <stdio.h>
#include <string.h>
 
pthread_mutex_t mutex ;
int g_Cnt ; 

#define PRINT_ERR(str, err) \
	{\
		char buf[1024] ; \
		if( strerror_r(err, buf, sizeof(buf)) == 0 ) { \
			fprintf(stderr, "%s, %d(%s)\n", str, err, buf ) ; \
		} \
		else { \
			fprintf(stderr, "%s, %d\n", str, err ) ; \
		} \
	}

void *thread_foo(void *ptr) {
	int result;
	char buf[512] ;

	sleep(1) ;
	if ((result = pthread_mutex_lock(&mutex)) != 0) {
		PRINT_ERR("pthread_mutex_lock fail", result) ;
		return NULL ;
	}

	g_Cnt++ ;

	if ((result = pthread_mutex_unlock(&mutex)) != 0) {
		PRINT_ERR("pthread_mutex_unlock fail", result) ;
		return NULL ;
	}
#if 0
	if( (result = pthread_mutex_destroy(&mutex)) < 0 ) {
		PRINT_ERR("pthread_mutex_destroy fail", result) ;
		return NULL ;
	}
#endif
	return 0;
} 

int main(void){
	pthread_t thr[5];
	size_t i ;
#if 1
	int ret;
#endif

	if( pthread_mutex_init(&mutex, NULL) != 0 ) {
		printf("pthread_mutex_init fail\n" ) ;
		return 1 ;
	}

	g_Cnt = 0 ;

	for( i = 0; i < 5 ; i++ ) {
		if (pthread_create(&thr[i], NULL, thread_foo,NULL) != 0) {
			fprintf(stderr, "Creation of thread failed\n");
			return 1 ;
		}
	}

	for( i = 0; i < 5 ; i++ ) {
		if( pthread_join( thr[i], NULL ) != 0 ) {
			fprintf(stderr, "thread join fail\n" ) ;
		}
	}
#if 1
	if( ( ret = pthread_mutex_destroy(&mutex)) < 0 ) {
		PRINT_ERR("pthread_mutex_destroy fail", ret) ;
		return -1;
	}
#endif
	printf("Cnt:%d\n", g_Cnt) ;

	return 0;
}
