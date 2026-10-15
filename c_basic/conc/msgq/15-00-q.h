#ifndef _15_00_H_
#define _15_00_H_

#include <stdbool.h>
#include <pthread.h>

#define MSG_Q_MAX_CNT	5
#define MODIFY 1
typedef struct _msg_q_s_
{
	int		front ;
	int		rear ;
	int		cur_cnt ;
#if MODIFY
	/* pbuf[] 는 flexible array member 라서 반드시 맨 마지막이어야 함 */
	pthread_mutex_t lock;
	pthread_cond_t not_empty;
	pthread_cond_t not_full;
	bool closed;
#endif
	void	*pbuf[] ;
} msg_q_t ;

typedef enum _msgq_rc_e_
{
	MSGQ_RC_SUCC = 0 ,
	MSGQ_RC_ERR_EMPTY= -1,
	MSGQ_RC_ERR_FULL = -2,
	MSGQ_RC_ERR_INVALID_PARAM = -3 ,
	MSGQ_RC_ERR_SYSFAIL = -4 ,
#if MODIFY
	MSGQ_RC_ERR_CLOSED = -5 ,
#endif
	MSGQ_RC_MAX
} msgq_rc_e ;


int init_q( msg_q_t **pp_q ) ;
bool is_empty_q( msg_q_t *p_q ) ;
bool is_full_q( msg_q_t *p_q ) ;
int enq( msg_q_t *p_q, void *p_data ) ;
int deq( msg_q_t *p_q, void **pp_data ) ;
void destroy_q( msg_q_t *p_q ) ;
#if MODIFY
void close_q( msg_q_t *p_q ) ;
#endif


#endif /* _15_00_H_ */
