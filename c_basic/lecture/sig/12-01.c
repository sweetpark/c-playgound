#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t sig1 = 0;
volatile sig_atomic_t sig2 = 0;

void handler(int signum) {
	if (signum == SIGUSR1) {
		printf("1\n") ;
		sig1 = 1;
	}
	else if (sig1) {
		printf("2\n") ;
		sig2 = 1;
	}
}

/* SIGUSR1 -> SIGUSR2 의도
 * SIGUSR2의 실행 시점에 따라서 SIGUSR1가 무시될 수 있다 (중간에 끊기거나)
 * */
int main(void) {
	if (signal(SIGUSR1, handler) == SIG_ERR){
		printf("signal(SIGUSR1) fail\n");
		return 1 ;
	}
	if (signal(SIGUSR2, handler) == SIG_ERR){
		printf("signal(SIGUSR1) fail\n");
		return 1 ;
	}

	while (sig2 == 0) {
		sleep(1) ;
	}

	return 0;
} 
