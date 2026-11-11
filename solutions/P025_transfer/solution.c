#include <pthread.h>
#include <stddef.h>

/* P025 transfer — 모범답안 */
typedef struct {
	int id;
	pthread_mutex_t mu;
	long bal;
} acct_t;

void acct_init(acct_t *a, int id, long bal)
{
	a->id = id;
	pthread_mutex_init(&a->mu, NULL);
	a->bal = bal;
}

int transfer(acct_t *from, acct_t *to, long amt)
{
	acct_t *first, *second;
	int rc = -1;

	if (from == NULL || to == NULL || from == to || amt <= 0)
		return -1;

	/* 락 획득 순서를 id 오름차순으로 고정 (A->B 와 B->A 가 동시에 와도 교착하지 않는다) */
	if (from->id < to->id) { first = from; second = to; }
	else                   { first = to;   second = from; }

	pthread_mutex_lock(&first->mu);
	pthread_mutex_lock(&second->mu);
	if (from->bal >= amt) {            /* 잔액 확인과 차감이 같은 락 안 */
		from->bal -= amt;
		to->bal += amt;
		rc = 0;
	}
	pthread_mutex_unlock(&second->mu); /* 획득의 역순 */
	pthread_mutex_unlock(&first->mu);
	return rc;
}
