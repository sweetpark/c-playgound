#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int gi_int = 0 ;
void handler(int signum) 
{
	gi_int++ ;
}

int main(void)
{
	if(signal(SIGUSR1, handler) == SIG_ERR) 
	{
		printf("signal() fail\n") ;
		return -1;
	}

	while( gi_int <= 1 )
	{
		printf("gi_int=%d\n", gi_int) ;

		raise(SIGUSR1) ;

		sleep(1) ;
	}

	return 0;
}
