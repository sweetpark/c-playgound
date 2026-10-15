#include <pthread.h>
#include <stdio.h>
#include <string.h>
 
void *thread_foo(void *ptr) {
	char *msg = (char *)ptr ;
	int cnt = 1 ;
	if( msg == NULL )
		return NULL ;

	char *tok ;

	printf("[%u] orig str=%s\n", pthread_self(), msg ) ;

	tok = strtok(msg, "/" ) ;
	if( tok == NULL ) {
		printf("[%u] String is not path name\n", pthread_self() ) ;
	}

	do {
		printf("[%u:%d] %s\n", pthread_self(), cnt++, tok ) ;
	} while( (tok = strtok(NULL, "/" ) ) != NULL ) ;

} 

enum { MAX_THREAD_NUM = 5 } ;
int main(void){
	pthread_t thr[MAX_THREAD_NUM];
	size_t i ;
	char str[MAX_THREAD_NUM][1024] = {"/home/user1/bin/httpd/conf/etc/file", "/home/uesr2/etc/conf/user3/", "/usr/include/xtd/ssh/etc/conf/get", "/home/user4/uesr/docker/etc/conf/dat", "/usr/lib/docker/man/help/ver1/data" } ;
	
	for( i = 0; i < MAX_THREAD_NUM ; i++ ) {
		if (pthread_create(&thr[i], NULL, thread_foo, &str[i][0]) != 0) {
			fprintf(stderr, "Creation of thread failed\n");
			return 1 ;
		}
	}

	for( i = 0; i < MAX_THREAD_NUM ; i++ ) {
		if( pthread_join( thr[i], NULL ) != 0 ) {
			fprintf(stderr, "thread join fail\n" ) ;
		}
	}

	return 0;
}
