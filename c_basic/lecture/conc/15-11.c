#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
 
typedef struct account_s {
	pthread_mutex_t balance_mutex ;
	unsigned int balance ;
	unsigned int id ;
} account ;

typedef struct transaction_s {
	unsigned int amount ;
	account *from ;
	account *to ;
} transaction ;

void *deposit(void *ptr) {
	transaction *args = (transaction *)ptr;

	printf("[%u] deposit start\n", pthread_self() ) ;
	sleep(1);
	printf("[%u] 1st lock try\n", pthread_self() ) ;
	if (0 != pthread_mutex_lock(&args->from->balance_mutex)) {
		printf("pthread_mutex_lock fail\n") ;
		goto end;
	}

	printf("[%u] 1st lock complete\n", pthread_self() ) ;
	/* Not enough balance to transfer */
	if (args->from->balance < args->amount) {
		printf("[%u] Not enough balance[%d] \n", pthread_self(), args->from->id ) ;
		if (0 != pthread_mutex_unlock(&args->from->balance_mutex)) {
			printf("pthread_mutex_lock fail\n") ;
		}
		goto end;
	}
	printf("[%u] 2nd lock try\n", pthread_self() ) ;

	if (0 != pthread_mutex_lock(&args->to->balance_mutex)) {
		printf("pthread_mutex_lock fail\n") ;
		/* from->balance_mutex unlock 필요 */
		goto end;
	}
	printf("[%u] 2nd lock complete\n", pthread_self() ) ;

	printf("%d -> %d amount:%d\n", args->from->id, args->to->id, args->amount ) ;
	args->from->balance -= args->amount;
	args->to->balance += args->amount;

	printf("[%d] %d, [%d] %d\n", args->from->id, args->from->balance, args->to->id, args->to->balance ) ;

	if (0 != pthread_mutex_unlock(&args->from->balance_mutex)) {
		printf("pthread_mutex_unlock fail\n") ;
		goto end;
	}

	if (0 != pthread_mutex_unlock(&args->to->balance_mutex)) {
		printf("pthread_mutex_unlock fail\n") ;
		goto end;
	}

	printf("[%u] deposit end\n", pthread_self() ) ;

end:
	free(ptr) ;

	return NULL;
}

enum {ACCOUNT_MAX=4} ;
account userAccounts[ACCOUNT_MAX] ;

int initAccounts() {
	size_t i ;
	
	for( i = 0 ; i < ACCOUNT_MAX ; i++ ) {
		userAccounts[i].id = i+1 ;
		userAccounts[i].balance = (i+1)*1000000 ;
		if( pthread_mutex_init( &userAccounts[i].balance_mutex, NULL ) != 0 ) {
			printf("pthread_mutex_init fail\n" ) ;
			return -1 ;
		} 
	}

	return 1 ;
}

void destroyAccounts() {
	size_t i ;
	for( i = 0 ; i < ACCOUNT_MAX ; i++ ) {
		if( pthread_mutex_destroy( &userAccounts[i].balance_mutex ) != 0 ) {
			printf("pthread_mutex_destroy fail\n" ) ;
		}
	}
}

transaction *create_tr( account *from, account *to, unsigned int amount ) {
	transaction *tr ;

	tr = (transaction *)malloc(sizeof(transaction)) ;
	if( tr == NULL ) {
		printf("malloc fail\n" ) ;
		return NULL ;
	}

	tr->from = from ;
	tr->to = to ;
	tr->amount = amount ;

	return tr ;
}

int main(void){
	pthread_t thr[2];
	size_t i ;
	transaction *tr[2] = {NULL, NULL};
	int ret ;

	if( initAccounts() < 0 ) {
		printf("initAccounts() fail\n" ) ;
		return 1 ;
	}

	tr[0] = create_tr( &userAccounts[0], &userAccounts[1], 10000 ) ;
	if( tr[0] == NULL ) {
		printf("create_tr fail\n") ;
		ret = 1 ;
		goto fail_end_2;
	}

	tr[1] = create_tr( &userAccounts[1], &userAccounts[0], 5000 ) ;
	if( tr[1] == NULL ) {
		printf("create_tr fail\n") ;
		ret = 1 ;
		goto fail_end_1 ;
	}

	for( i = 0; i < 2 ; i++ ) {
		if (pthread_create(&thr[i], NULL, deposit, tr[i] ) != 0) {
			printf("Creation of thread failed\n");
			ret = 1 ;
			goto fail_end_1 ;
		}
		tr[i] = NULL ;
	}

	for( i = 0; i < 2 ; i++ ) {
		if( pthread_join( thr[i], NULL ) != 0 ) {
			printf("pthread_join failed\n");
			ret = 1 ;
			goto fail_end_2 ;
		}
	}

	destroyAccounts() ;

	return 0 ;

fail_end_1:
	for( i = 0; i < 2; i++ ) {
		if( tr[i] != NULL )
			free(tr[i]) ;
	}

fail_end_2:
	destroyAccounts() ;

	return ret;
}
