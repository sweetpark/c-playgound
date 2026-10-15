#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "15-00-q.h"

msg_q_t	*g_msgq;

#if MODIFY
#include <signal.h>

sigset_t sigset;
pthread_t sig_thr;

void *sig_thr_func(void *arg){
	int sig = 0 ;
	int rc = sigwait(&sigset, &sig);

	if( rc != 0 ){
		printf("sigwait fail, rc=%d\n", rc) ;
		close_q(g_msgq);
		return NULL;
	}

	if(sig == SIGINT || sig == SIGTERM){
		printf("signal(%d) received, shutting down\n", sig) ;
		close_q(g_msgq);
	}
	/* SIGUSR1 : 정상 종료 시 main 이 깨운 것 -> 아무 것도 안 함 */

	return NULL;
}

#endif

#define MAX_THREAD_COUNT	5
#define MAX_WAITING_TIME	10000
//msg_q_t	*g_msgq;

bool my_atoi(char *str, int *dst)
{
	long tmp ;
	char *endptr ;

	if( str == NULL || dst == NULL )
	{
		printf("invalid arg\n") ;
		return false ;
	}
	else if( strlen(str) == 0 ) 
	{
		printf("str's length is 0\n") ;
		return false ;
	}

	errno = 0;
	tmp = strtol(str, &endptr, 10) ;
	if( errno == EINVAL ) 
	{
		printf("strtol is fail, (invalid)\n") ;
		return false;
	}
	else if( (tmp == LONG_MAX || tmp == LONG_MIN ) && errno == ERANGE ) 
	{
		printf("overflow or underflow\n") ;
		return false ;
	}
	else if( *endptr != '\0' )
	{
		printf("invalid character\n") ;
		return false ;
	}
	else if( INT_MAX < tmp || INT_MIN > tmp )
	{
		printf("overflow or underflow\n") ;
		return false ;
	}

	*dst = (int)tmp ;

	return true ;
}

#if MODIFY
void msleep( int msec )
{
	struct timespec tm ;

	/* tv_nsec 는 1e9 미만이어야 하므로 초/나노초로 분리 (기존 msec*1000 은 usec 단위였음) */
	tm.tv_sec = msec / 1000 ;
	tm.tv_nsec = (long)(msec % 1000) * 1000000L ;

	nanosleep(&tm, NULL) ;
}
#else
void msleep( int msec )
{
	struct timespec tm ;

	tm.tv_sec = 0 ;
	tm.tv_nsec = msec * 1000 ;

	nanosleep(&tm, NULL) ;
}
#endif

#if MODIFY
void producer(int wait_time, int repeat_cnt, int consumer_cnt )
{
	int i ;
	char *pdata ;
	char ch;
	int rc ;

	/* 어떤 경로로 빠져나가도 close_q 를 호출해야 consumer 가 영원히 대기하지 않음 */
	if ( wait_time <  0 )
	{
		printf("wait_time(%d) is invalid\n", wait_time) ;
		goto end ;
	}

	if( repeat_cnt <= 0 || repeat_cnt > 100 )
	{
		printf("repet_cnt(%d) is invalid( 0 < repeat_cnt <= 100 )\n", repeat_cnt) ;
		goto end ;
	}

	for( i = 0; i < repeat_cnt  ; i++ )
	{
		for( ch = 'a'; ch < 'z'; ch++ )
		{
			pdata = (char *)malloc(1) ;
			if (pdata == NULL)
			{
				printf("malloc_fail\n") ;
				goto end;
			}

			*pdata = ch ;

			/* 가득 차면 enq 가 내부에서 블로킹 대기 (polling 불필요) */
			rc = enq(g_msgq, (void *)pdata) ;
			if( rc != MSGQ_RC_SUCC )
			{
				if( rc != MSGQ_RC_ERR_CLOSED )	/* CLOSED 는 SIGINT 등에 의한 정상 종료 경로 */
				{
					printf("enq(%c) fail, rc=%d\n", *pdata, rc) ;
				}
				free(pdata) ;
				goto end ;
			}

			msleep(wait_time) ;
		}
	}

end:
	/* 'Q' 센티널 대신 close: 남은 데이터는 consumer 가 모두 처리한 뒤 종료 */
	close_q(g_msgq) ;
}
#else
void producer(int wait_time, int repeat_cnt, int consumer_cnt )
{
	int i ;
	char *pdata ;
	char ch;
	int rc ;

	if ( wait_time <  0 )
	{
		printf("wait_time(%d) is invalid\n", wait_time) ;
		return ;
	}

	if( repeat_cnt <= 0 || repeat_cnt > 100 ) 
	{
		printf("repet_cnt(%d) is invalid( 0 < repeat_cnt <= 100 )\n", repeat_cnt) ;
		return ;
	}

	for( i = 0; i < repeat_cnt  ; i++ )
	{
		for( ch = 'a'; ch < 'z'; ch++ )
		{
			pdata = (char *)malloc(1) ;
			if (pdata == NULL) 
			{
				printf("malloc_fail\n") ;
				goto end;
			}

			*pdata = ch ;

			while(1) {
				rc = enq(g_msgq, (void *)pdata) ;
				if( rc == MSGQ_RC_ERR_FULL )
				{
					msleep(1000) ;
					continue ;
				}
				else if(rc  != MSGQ_RC_SUCC )
				{
					printf("enq(%c) fail, rc=%d\n", *pdata, rc) ;
					free(pdata) ;
					goto end ;
				}

				break ;
			}

			msleep(wait_time) ;
		}
	}

end:
	for( i = 0 ; i < consumer_cnt; i++ )
	{
		pdata = (char *)malloc(1) ;
		if( pdata ==  NULL )
		{
			printf("malloc fail\n");
		}
		else
		{
			*pdata = 'Q' ;
#define		MAX_RETRY_CNT	10
			int retry_cnt = 0 ;
			int rc = MSGQ_RC_SUCC ; 
			while( retry_cnt < MAX_RETRY_CNT )
			{
				rc = enq(g_msgq, (void *)pdata) ;
				if(  rc != MSGQ_RC_SUCC )
				{
					if( rc == MSGQ_RC_ERR_FULL ) 
					{
						retry_cnt++ ;
						msleep(1000) ;
						continue ;
					}
					else
					{
						printf("enq(%c) fail\n", *pdata ) ;
						free(pdata) ;
						pdata = NULL ;
						break ;
					}
				}
				else
				{
					break ;
				}
			}
			if( retry_cnt >= MAX_RETRY_CNT )
			{
					printf("enq(%c) fail, retry(%d) fail\n", *pdata, retry_cnt ) ;
				if( pdata != NULL ) 
				{
					free(pdata) ;
					pdata = NULL ;
				}
			}
				
		}
	}
}

#endif

#if MODIFY
void *consumer_thr(void *arg)
{
	int thr_id = (int)(intptr_t) arg ;
	char *pdata ;
	int rc ;

	printf("thread_%d start\n", thr_id) ;
	while(1)
	{
		rc = deq(g_msgq, (void **)&pdata) ;
		if( rc == MSGQ_RC_ERR_CLOSED )
		{
			break ;
		}
		else if( rc != MSGQ_RC_SUCC )
		{
			printf("deq fail, rc=%d\n", rc) ;
			break ;
		}

		printf("[%d] recv : %c\n", thr_id, *pdata) ;

		free(pdata) ;
	}

	printf("thread_%d stop\n", thr_id) ;
	return NULL ;
}
#else
void *consumer_thr(void *arg)
{
	int thr_id = (int) arg ;
	char *pdata ;
	int rc ;
	int cflag = 1 ;

	printf("thread_%d start\n", thr_id) ;
	while(cflag)
	{
		rc = deq(g_msgq, (void **)&pdata) ;
		if( rc == MSGQ_RC_ERR_EMPTY ) 
		{
			msleep(1000) ;
			continue;
		}
		else if( rc != MSGQ_RC_SUCC )
		{
			printf("deq fail\n") ;
			return ;
		}

		printf("[%d] recv : %c\n", thr_id, *pdata) ;
		
		if( *pdata == 'Q' )
		{
			cflag = 0 ;
		}

		free(pdata) ;
	}

	printf("thread_%d stop\n", thr_id) ;
}
#endif

/*
 * enq시에 queue 가 가득 차 있으면 wait -> 공간생기면 enq
 * deq시에 queue 가 비어있으면 wait -> 줄게 생기면 deq
 * producer 정해진 횟수만큼 수행 이후 종료 대신,  SIGINT 수신시 프로세스 종료 절차로 수정
 */


int main(int argc, char *argv[])
{
	if( argc != 3 )
	{
		printf("Usage:%s num1 num2 (num1 is consumer thread count, num2 is producing waiting time(msec)\n", argv[0] ) ;
		return -1 ;
	}

	int consumer_cnt = 0 ;
	int waiting_time = 0 ;

	if( !my_atoi(argv[1], &consumer_cnt) )
	{
		printf("%s is invalid\n", argv[1]);
		return -1 ;
	}
	else if( consumer_cnt < 1 || consumer_cnt > MAX_THREAD_COUNT )
	{
		printf("consumer count(%d) is invalid(1<=consumer_cnt<=%d)\n", consumer_cnt, MAX_THREAD_COUNT);
		return -1 ;
	}

	if( !my_atoi(argv[2], &waiting_time) )
	{
		printf("%s is invalid\n", argv[2]);
		return -1 ;
	}
	else if( waiting_time < 0 || waiting_time > MAX_WAITING_TIME )
	{
		printf("wating time(%d) is invalid(0<=waiting_time<=%d)\n", waiting_time, MAX_WAITING_TIME);
		return -1 ;
	}

	printf("consumer_cnt:%d, waiting_time:%d ms\n",  consumer_cnt, waiting_time);



	int rc = init_q(&g_msgq) ;
	if( rc != MSGQ_RC_SUCC ) 
	{
		printf("init_q fail, rc=%d\n", rc) ;
		return -1 ;
	}


	pthread_t thr[MAX_THREAD_COUNT] ;


#if MODIFY
	int i ;
	int created = 0 ;		/* consumer 스레드 수 */
	bool sig_created = false ;

	/* 스레드를 만들기 전에 막아야 모든 스레드가 이 마스크를 상속 */
	sigemptyset(&sigset);
	sigaddset(&sigset,SIGINT);
	sigaddset(&sigset,SIGTERM);
	sigaddset(&sigset,SIGUSR1);
	pthread_sigmask(SIG_BLOCK, &sigset, NULL);

	rc = pthread_create(&sig_thr, NULL, sig_thr_func, NULL);
	if( rc != 0 )
	{
		printf("pthread_create(sig_thr) fail, rc=%d\n", rc) ;
	}
	else
	{
		sig_created = true ;
	}

	for( i = 0 ; sig_created && i < consumer_cnt ; i++ )
	{
		rc = pthread_create(&thr[i], NULL, consumer_thr, (void *)(intptr_t)i) ;
		if( rc != 0 )
		{
			printf("pthread_create fail, rc=%d\n", rc) ;
			break ;
		}
		created++ ;
	}

	if( created == consumer_cnt )
	{
		producer(waiting_time, 2, consumer_cnt) ;
	}
	else
	{
		/* 이미 만든 스레드가 대기 중이므로 깨워서 종료시킨 뒤 join */
		close_q( g_msgq ) ;
	}

	for( i = 0; i < created ; i++ )
	{
		pthread_join(thr[i], NULL) ;
	}

	if( sig_created )
	{
		/* 시그널 없이 끝난 경우 sigwait 에서 기다리는 스레드를 깨워서 join */
		pthread_kill(sig_thr, SIGUSR1) ;
		pthread_join(sig_thr, NULL) ;
	}

	printf("Process Done!!!\n") ;

	destroy_q( g_msgq ) ;

	return ( created == consumer_cnt ) ? 0 : -1 ;
#else
	int i ;
	for( i = 0 ; i < consumer_cnt ; i++ )
	{
		rc = pthread_create(&thr[i], NULL, consumer_thr, (void *)i) ;
		if( rc != 0 )
		{
			printf("pthread_create fail, rc=%d\n", rc) ;
			return -1 ;
		}
	}

	producer(waiting_time, 2, consumer_cnt) ;

	for( i = 0; i < consumer_cnt ; i++ )
	{
		pthread_join(thr[i], NULL) ;
	}

	printf("Process Done!!!") ;

	destroy_q( g_msgq ) ;

	return 0 ;
#endif
}
