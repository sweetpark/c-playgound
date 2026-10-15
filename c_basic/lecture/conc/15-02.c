#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
 
enum { MIN_BALANCE = 50 };

int account_balance;
pthread_mutex_t mp;

int verify_balance(int amount) {
	if (account_balance - amount < MIN_BALANCE) {
		printf("Low balance(%d)\n", account_balance) ;
		if (pthread_mutex_unlock(&mp) != 0) {
			printf("pthread_mutex_unlock fail\n") ;
		}
		// return 문이 pthread_mutex_unlock if 구문 안에 있어야함
		return -1;
	}
	return 0;
}

void debit(int amount) {
	if (pthread_mutex_lock(&mp) != 0) {
		printf("pthread_mutex_lock fail\n") ;
		return ;
	}
	if (verify_balance(amount) == -1) {
		// unlock 호출 2번
		if (pthread_mutex_unlock(&mp) != 0) {
			printf("pthread_mutex_unlock fail\n") ;
		}
		return;
	}
	account_balance -= amount;
	printf("balance(%d)\n", account_balance) ;
	if (pthread_mutex_unlock(&mp) !=0) {
		printf("pthread_mutex_unlock fail\n") ;
	}
}

void *account_debit(void *ptr) {
	int cnt ;

	for( cnt = 0; cnt < 10; cnt++ ) {
		debit(10) ;
	}

	return NULL ;
}

int main(void){
	pthread_t thr[2];
	size_t i ;
	int ret ;

	if( pthread_mutex_init(&mp, NULL) != 0 ) {
		printf("pthread_mutex_init fail\n" ) ;
		return 1 ;
	}

	account_balance = 200 ;

	for ( i = 0; i < 2; ++i) {
		if (pthread_create(&(thr[i]), NULL, account_debit, NULL) != 0) {
			fprintf(stderr, "Creation of thread %zu failed\n", i);
			return ;
		}
	}

	for( i = 0; i < 2; ++i ) {
		if( pthread_join(thr[i], NULL ) != 0 ) {
			printf("pthread_join fail\n") ;
		}
	}

	if( ( ret = pthread_mutex_destroy(&mp) ) != 0 ) {
		char buf[1024] ;
		strerror_r(ret, buf, 1024) ;
		printf("pthread_mutex_destroy fail, ret=%d(%s)\n", ret, buf) ;
		return 1 ;
	}

	return 0;
}
