#include <pthread.h>
#include <stdio.h>
 
pthread_mutex_t mutex ;
static int a = 0 ;
static int b = 0 ;

int set_values(int new_a, int new_b) {
	/*
	if (pthread_mutex_lock(&mutex) != 0) {
		fprintf(stderr, "pthread_mutex_lock fail\n") ;
		return -1 ;	
	}
*/
	a = new_a ;
	b = new_b ;
/*
	if (pthread_mutex_unlock(&mutex) != 0) {
		fprintf(stderr, "pthread_mutex_unlock fail\n") ;
		return -1 ;
	}
*/
	return 1 ;
}

int get_sum(int *sum) {
	if( sum == NULL ) return -1 ;
/*
	if (pthread_mutex_lock(&mutex) != 0) {
		fprintf(stderr, "pthread_mutex_lock fail\n") ;
		return -1 ;	
	}
*/
	*sum = a + b ;
/*
	if (pthread_mutex_unlock(&mutex) != 0) {
		fprintf(stderr, "pthread_mutex_unlock fail\n") ;
		return -1 ;
	}
*/
	return 1 ;
}

int get_product(int *product) {
	if( product == NULL ) return -1 ;
/*
	if (pthread_mutex_lock(&mutex) != 0) {
		fprintf(stderr, "pthread_mutex_lock fail\n") ;
		return -1 ;	
	}
*/
	*product = a * b ;
/*
	if (pthread_mutex_unlock(&mutex) != 0) {
		fprintf(stderr, "pthread_mutex_unlock fail\n") ;
		return -1 ;
	}
*/
	return 1 ;
}

int multiply_monomials(int x1, int x2) {
	int sum, product ;

#if 1
	if(pthread_mutex_lock(&mutex) != 0){
		fprintf(stderr, "pthread_mutex_lock fail\n");
		return -1;
	}
#endif

	set_values(x1, x2) ;
	printf("[%u] (x+%d)(x+%d)\n", pthread_self(), x1, x2) ;
	
	if( get_sum( &sum ) < 0 ) {
		return -1 ;
	}

	if(get_product( &product ) < 0 ) {
		return -1 ;
	}

#if 1
	if(pthread_mutex_unlock(&mutex)!=0){
		fprintf(stderr, "pthread_mutex_unlock fail\n");
		return -1;
	}
#endif
	printf("[%u] = x^2 + %dx + %d\n", pthread_self(), sum, product ) ;

	return 1;
}

void *thread_foo(void *ptr) {
	sleep(1) ;
	int n = (int)((long)ptr);

	if( multiply_monomials( n+1, n+2 ) < 0 ) {
		printf("multiply_monomials fail\n" ) ;
		return NULL;
	}

	return NULL;
} 

int main(void){
	pthread_t thr[10];
	size_t i ;
	size_t thrno = 10 ;
	if( pthread_mutex_init(&mutex, NULL) != 0 ) {
		printf("pthread_mutex_init fail\n" ) ;
		return 1 ;
	}

	for( i = 0; i < thrno ; i++ ) {
		if (pthread_create(&thr[i], NULL, thread_foo,(void *)i) != 0) {
			fprintf(stderr, "Creation of thread failed\n");
			return 1 ;
		}
	}

	for( i = 0; i < thrno ; i++ ) {
		if( pthread_join( thr[i], NULL ) != 0 ) {
			fprintf(stderr, "thread join fail\n" ) ;
		}
	}

	if( pthread_mutex_destroy(&mutex) != 0 ) {
		printf("pthread_mutex_destroy fail\n" ) ;
		return 1 ;
	}

	return 0;
}
