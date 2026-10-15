#include <pthread.h>
#include <stdio.h>
 
pthread_mutex_t mutex ;
int g_accessCnt ; 

void *thread_foo(void *ptr) {
	int result;
	FILE *fp;
	char buf[512] ;

	if ((result = pthread_mutex_lock(&mutex)) != 0) {
		fprintf(stderr, "pthread_mutex_lock fail\n") ;
		return NULL ;
	}

	printf("Thread[%d]==================\n", pthread_self() ) ;
	fp = fopen("SomeNetworkFile", "r");
	if (fp != NULL) {
		while( fgets(buf, 512, fp) != NULL ) {
			printf("%s", buf) ;
		}
		printf("End====================\n") ;
		fclose(fp);
	}
	// 읽기 모드의 경우 I/O를 lock 을 잡으면 성능이 급격하게 내려감 -> g_accessCnt 만 lock 처리 필요
	g_accessCnt++ ;

	if ((result = pthread_mutex_unlock(&mutex)) != 0) {
		fprintf(stderr, "pthread_mutex_unlock fail\n") ;
		return NULL ;
	}

	return 0;
} 

int main(void){
	pthread_t thr[2];
	size_t i ;

	if( pthread_mutex_init(&mutex, NULL) != 0 ) {
		printf("pthread_mutex_init fail\n" ) ;
		return 1 ;
	}

	g_accessCnt = 0 ;

	for( i = 0; i < 2 ; i++ ) {
		if (pthread_create(&thr[i], NULL, thread_foo,NULL) != 0) {
			fprintf(stderr, "Creation of thread failed\n");
			return 1 ;
		}
	}

	for( i = 0; i < 2 ; i++ ) {
		if( pthread_join( thr[i], NULL ) != 0 ) {
			fprintf(stderr, "thread join fail\n" ) ;
		}
	}

	if( pthread_mutex_destroy(&mutex) != 0 ) {
		printf("pthread_mutex_destroy fail\n" ) ;
		return 1 ;
	}

	printf("Total Access Cnt:%d\n", g_accessCnt) ;

	return 0;
}
