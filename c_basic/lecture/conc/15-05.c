#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define USE_MUTEX	1	/* 0 으로 하면 mutex 없이 빌드 (레이스 확인용) */
#define THR_CNT		10

static bool flag = false ;
#if USE_MUTEX
pthread_mutex_t lock;
#endif

/* 토글하고 "내가 바꾼 결과" 값을 같은 임계구역에서 반환 (다른 스레드가 끼어들 수 없음) */
bool toggle_flag(void) {
	bool ret ;

#if USE_MUTEX
	pthread_mutex_lock(&lock);
#endif
	flag = !flag ;
	ret = flag ;
#if USE_MUTEX
	pthread_mutex_unlock(&lock);
#endif

	return ret ;
}

bool get_flag(void) {
	bool ret ;

	// 공유변수 접근
#if USE_MUTEX
	pthread_mutex_lock(&lock);
#endif
	ret = flag ;
#if USE_MUTEX
	pthread_mutex_unlock(&lock);	/* return 보다 먼저 unlock */
#endif

	return ret ;
}

void *thread_foo(void *ptr) {
	bool now ;

	sleep(1) ;
	printf("[%lu] cur flag:%d\n", (unsigned long)pthread_self(), get_flag() ) ;
	now = toggle_flag() ;
	printf("[%lu] toggle flag:%d\n", (unsigned long)pthread_self(), now ) ;

	return NULL ;
}

int main(void){
	pthread_t thr[THR_CNT];
	size_t i ;
	int created = 0 ;
	int ret = 0 ;

#if USE_MUTEX
	if(pthread_mutex_init(&lock, NULL) != 0){	/* 실패 시 errno 값(양수) 반환 */
		printf("mutex init failed\n");
		return -1;
	}
#endif

	for( i = 0; i < THR_CNT ; i++ ) {
		if (pthread_create(&thr[i], NULL, thread_foo,NULL) != 0) {
			fprintf(stderr, "Creation of thread failed\n");
			ret = 1 ;
			break ;
		}
		created++ ;
	}

	/* 생성에 성공한 스레드만 join */
	for( i = 0; i < (size_t)created ; i++ ) {
		if( pthread_join( thr[i], NULL ) != 0 ) {
			fprintf(stderr, "thread join fail\n" ) ;
		}
	}

	printf("final flag:%d\n", flag) ;

#if USE_MUTEX
	if(pthread_mutex_destroy(&lock) != 0){
		printf("mutex destroy failed\n");
		return -1;
	}
#endif

	return ret;
}
