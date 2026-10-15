#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include "15-00-q.h"

int init_q( msg_q_t **pp_q )
{
	msg_q_t *p_q ;

	if( pp_q == NULL )
	{
		printf("invalid pp_q\n") ;
		return MSGQ_RC_ERR_INVALID_PARAM ;
	}

	p_q = (msg_q_t *)malloc(sizeof(msg_q_t)+sizeof(void *)*MSG_Q_MAX_CNT) ;
	if( p_q == NULL)
	{
		printf("malloc fail<%d:%s>\n", errno, strerror(errno) ) ;
		return MSGQ_RC_ERR_SYSFAIL ;
	}

	p_q->front = 0;
	p_q->rear = 0 ;
	p_q->cur_cnt = 0 ;

	int n ;
	for(n = 0 ; n < MSG_Q_MAX_CNT; n++)
	{
		p_q->pbuf[n] = NULL ;
	}

#if MODIFY
	p_q->closed = false ;

	if( pthread_mutex_init(&p_q->lock, NULL) != 0 )
	{
		printf("mutex init fail\n") ;
		free(p_q) ;
		return MSGQ_RC_ERR_SYSFAIL ;
	}

	if( pthread_cond_init(&p_q->not_empty, NULL) != 0 )
	{
		printf("cond(not_empty) init fail\n") ;
		pthread_mutex_destroy(&p_q->lock) ;
		free(p_q) ;
		return MSGQ_RC_ERR_SYSFAIL ;
	}

	if( pthread_cond_init(&p_q->not_full, NULL) != 0 )
	{
		printf("cond(not_full) init fail\n") ;
		pthread_cond_destroy(&p_q->not_empty) ;
		pthread_mutex_destroy(&p_q->lock) ;
		free(p_q) ;
		return MSGQ_RC_ERR_SYSFAIL ;
	}
#endif

	*pp_q = p_q ;
	return MSGQ_RC_SUCC ;
}

bool is_empty_q( msg_q_t *p_q )
{
	if( p_q == NULL )
	{
		printf("invalid p_q\n") ;

		return false ;
	}

#if MODIFY
	bool ret ;

	/* 조회 시점의 스냅샷일 뿐, "확인 후 동작" 용도로 쓰면 안 됨 (enq/deq 가 내부에서 직접 판단) */
	pthread_mutex_lock(&p_q->lock) ;
	ret = ( p_q->cur_cnt == 0 ) ;
	pthread_mutex_unlock(&p_q->lock) ;

	return ret ;
#else
	if( p_q->cur_cnt == 0 )
	{
		return true ;
	}
	else {
		return false ;
	}
#endif
}

bool is_full_q( msg_q_t *p_q )
{
	if( p_q == NULL )
	{
		printf("invalid p_q\n") ;

		return false ;
	}

#if MODIFY
	bool ret ;

	pthread_mutex_lock(&p_q->lock) ;
	ret = ( p_q->cur_cnt >= MSG_Q_MAX_CNT ) ;
	pthread_mutex_unlock(&p_q->lock) ;

	return ret ;
#else
	if( p_q->cur_cnt >= MSG_Q_MAX_CNT )
	{
		return true ;
	}
	else
	{
		return false;
	}
#endif
}

int enq( msg_q_t *p_q, void *p_data )
{
#if MODIFY
	if( p_q == NULL )
	{
		printf("invalid p_q \n");
		return MSGQ_RC_ERR_INVALID_PARAM ;
	}

	if( p_data == NULL )
	{
		printf("invalid p_data\n") ;
		return MSGQ_RC_ERR_INVALID_PARAM;
	}

	pthread_mutex_lock(&p_q->lock);

	/* 가득 차 있으면 자리가 날 때(또는 close) 까지 대기. spurious wakeup 때문에 반드시 while */
	while( p_q->cur_cnt >= MSG_Q_MAX_CNT && !p_q->closed )
	{
		pthread_cond_wait(&p_q->not_full, &p_q->lock);
	}

	if( p_q->closed )
	{
		pthread_mutex_unlock(&p_q->lock);
		return MSGQ_RC_ERR_CLOSED;
	}

	p_q->pbuf[p_q->rear] = p_data;
	p_q->rear = (p_q->rear+1) % MSG_Q_MAX_CNT;
	p_q->cur_cnt++;

	/* 새로 데이터 들어온 것을 알림 -> deq wake */
	pthread_cond_signal(&p_q->not_empty);
	pthread_mutex_unlock(&p_q->lock);

	return MSGQ_RC_SUCC;
#else
	int rc  ;

	if( p_q == NULL )
	{
		printf("invalid p_q \n");
		return MSGQ_RC_ERR_INVALID_PARAM ;
	}

	if( p_data == NULL )
	{
		printf("invalid p_data\n") ;
		return MSGQ_RC_ERR_INVALID_PARAM;
	}

	if( is_full_q( p_q) )
	{
		return MSGQ_RC_ERR_FULL ;
	}

	p_q->pbuf[p_q->rear] = p_data ;

	p_q->rear = (p_q->rear+1)%MSG_Q_MAX_CNT ;

	p_q->cur_cnt++ ;
	return MSGQ_RC_SUCC ;
#endif
}

int deq( msg_q_t *p_q, void **pp_data )
{
#if MODIFY
	if( p_q == NULL )
	{
		printf("invalid p_q \n");
		return MSGQ_RC_ERR_INVALID_PARAM ;
	}

	if( pp_data == NULL )
	{
		printf("invalid p_data\n") ;
		return MSGQ_RC_ERR_INVALID_PARAM ;
	}

	pthread_mutex_lock(&p_q->lock);

	while( p_q->cur_cnt == 0 && !p_q->closed )
	{
		pthread_cond_wait(&p_q->not_empty, &p_q->lock);
	}

	/* close 되었어도 남은 데이터는 모두 꺼낸 뒤에 CLOSED 를 돌려줌 (drain) */
	if( p_q->cur_cnt == 0 )
	{
		pthread_mutex_unlock(&p_q->lock);
		return MSGQ_RC_ERR_CLOSED ;
	}

	*pp_data = p_q->pbuf[p_q->front] ;
	p_q->pbuf[p_q->front] = NULL ;
	p_q->front = (p_q->front+1) % MSG_Q_MAX_CNT ;
	p_q->cur_cnt-- ;

	/* 공간이 생겼다는걸 알림 -> enq wake */
	pthread_cond_signal(&p_q->not_full);
	pthread_mutex_unlock(&p_q->lock);

	return MSGQ_RC_SUCC ;
#else
	int rc ;

	if( p_q == NULL )
	{
		printf("invalid p_q \n");
		return MSGQ_RC_ERR_INVALID_PARAM ;
	}

	if( pp_data == NULL )
	{
		printf("invalid p_data\n") ;
		return MSGQ_RC_ERR_INVALID_PARAM ;
	}

	if(is_empty_q(p_q))
	{
		return MSGQ_RC_ERR_EMPTY ;
	}

	*pp_data = p_q->pbuf[p_q->front] ;

	p_q->pbuf[p_q->front] = NULL ;

	p_q->front = (p_q->front+1)%MSG_Q_MAX_CNT ;

	p_q->cur_cnt-- ;

	return MSGQ_RC_SUCC ;
#endif
}

#if MODIFY
void close_q( msg_q_t *p_q )
{
	if( p_q == NULL )
	{
		printf("invalid p_q\n") ;
		return ;
	}

	pthread_mutex_lock(&p_q->lock);
	p_q->closed = true ;
	/* 대기 중인 producer/consumer 전원을 깨워야 하므로 signal 이 아니라 broadcast */
	pthread_cond_broadcast(&p_q->not_empty);
	pthread_cond_broadcast(&p_q->not_full);
	pthread_mutex_unlock(&p_q->lock);
}
#endif

void destroy_q( msg_q_t *p_q )
{
	if( p_q == NULL )
	{
		printf("invalid p_q\n") ;
		return ;
	}

	int n ;
	int idx = p_q->front ;
	for( n = 0; n < p_q->cur_cnt ; n++ )
	{
		if( p_q->pbuf[idx] != NULL )
		{
			free(p_q->pbuf[idx]) ;
			p_q->pbuf[idx] = NULL ;
		}

		idx = (idx+1)%MSG_Q_MAX_CNT ;
	}

#if MODIFY
	/* 모든 스레드가 join 된 뒤에만 호출해야 함 */
	pthread_cond_destroy(&p_q->not_full);
	pthread_cond_destroy(&p_q->not_empty);
	pthread_mutex_destroy(&p_q->lock);
#endif

	free(p_q) ;
}
