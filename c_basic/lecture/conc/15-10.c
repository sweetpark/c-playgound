#include <pthread.h>
#include <stdio.h>
 
void *child_thread(void *val) {
	int *res = (int *)val;
	printf("[%u] Result: %d\n", pthread_self(), *res);
	return NULL;
}

void create_thread(pthread_t *tid, int start) {
	int val = start+1;
	if (0 != pthread_create(tid, NULL, child_thread, &val)) {
		printf("pthread_create fail\n") ;
		return ;
	}
}

int main(void) {
	pthread_t tid[5] ;
	size_t i ;

	for( i = 0 ; i < 5 ; i++ ) {
		create_thread(&tid[i], i);
	}

	for( i = 0 ; i < 5 ; i++ ) {
		if (0 != pthread_join(tid[i], NULL)) {
			printf("pthread_join fail\n") ;
		}
	}

	return 0;
}
