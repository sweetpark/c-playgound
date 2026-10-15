#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

#define THR_CNT		4
#define LOOP_CNT	20
#define AMOUNT		100
#define INIT_BALANCE	2000

/* account_balance, debit_ok, credit_ok 는 반드시 lock 을 잡고 접근 (join 이후 main 제외) */
unsigned int account_balance;
unsigned int debit_ok;		/* 성공한 debit 횟수 (검증용) */
unsigned int credit_ok;		/* 성공한 credit 횟수 (검증용) */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int debit(unsigned int amount) {
	int rc = 1 ;
#if 1
	pthread_mutex_lock(&lock);
#endif
	if( account_balance < amount ) {
		rc = -1 ;
	} else {
		account_balance -= amount;
		debit_ok++ ;
	}
#if 1
	pthread_mutex_unlock(&lock);
#endif
	/* I/O 는 락 밖에서 */
	if( rc < 0 ) {
		printf("balance is low \n" ) ;
	}
	return rc ;
}

int credit(unsigned int amount ) {
	int rc = 1 ;
#if 1
	pthread_mutex_lock(&lock);
#endif
	if( (UINT_MAX - account_balance) < amount ) {
		rc = -1 ;
	} else {
		account_balance += amount ;
		credit_ok++ ;
	}
#if 1
	pthread_mutex_unlock(&lock);
#endif
	if( rc < 0 ) {
		printf("balance is large\n") ;
	}
	return rc ;
}

/* srand 는 프로세스 전역 상태라 쓰지 않고, 스레드마다 다른 seed 로 rand_r 사용 */
void rand_sleep(unsigned int seed)
{
	int r = rand_r(&seed) ;
	usleep( r % 100 ) ;
}

void *account_debit(void *ptr) {
	int cnt ;

	rand_sleep( (unsigned int)time(NULL) ^ (unsigned int)(intptr_t)ptr ) ;
	for( cnt = 0; cnt < LOOP_CNT; cnt++ ) {
		debit(AMOUNT) ;
	}
	return NULL ;
}

void *account_credit(void *ptr) {
	int cnt ;

	rand_sleep( (unsigned int)time(NULL) ^ (unsigned int)(intptr_t)ptr ) ;
	for( cnt = 0; cnt < LOOP_CNT; cnt++ ) {
		credit(AMOUNT) ;
	}

	return NULL ;
}

int main(void){
	pthread_t thr[THR_CNT];
	size_t i ;
	int created = 0 ;
	int ret = 0 ;

	account_balance = INIT_BALANCE ;

	printf( "start account_balance = %u\n", account_balance ) ;

	for ( i = 0; i < THR_CNT; i+=2) {
		if (pthread_create(&(thr[i]), NULL, account_debit, (void *)(intptr_t)(i+1)) != 0) {
			fprintf(stderr, "Creation of thread %zu failed\n", i);
			ret = -1 ;
			break ;
		}
		created++ ;

		if (pthread_create(&(thr[i+1]), NULL, account_credit, (void *)(intptr_t)(i+2)) != 0) {
			fprintf(stderr, "Creation of thread %zu failed\n", i+1);
			ret = -1 ;
			break ;
		}
		created++ ;
	}

	/* 생성에 성공한 스레드만 join */
	for( i = 0; i < (size_t)created; ++i ) {
		if( pthread_join(thr[i], NULL ) != 0 ) {
			printf("pthread_join fail\n") ;
		}
	}

	printf("result account_balance=%u\n", account_balance);

	if( ret == 0 ) {
		/* 레이스가 없으면 최종 잔액 = 초기값 + 입금 성공분 - 출금 성공분 */
		unsigned int expected = INIT_BALANCE + AMOUNT * credit_ok - AMOUNT * debit_ok ;
		if( expected != account_balance ) {
			printf("MISMATCH: expected=%u (debit_ok=%u, credit_ok=%u)\n", expected, debit_ok, credit_ok) ;
			ret = -1 ;
		} else {
			printf("check ok (debit_ok=%u, credit_ok=%u)\n", debit_ok, credit_ok) ;
		}
	}

	pthread_mutex_destroy(&lock);
	return ret ;
}
