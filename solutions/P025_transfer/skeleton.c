#include <pthread.h>
#include <stddef.h>

/* P025 transfer — 요구사항은 problem.md */
typedef struct {
	int id;                 /* 계좌 번호: 락 순서의 기준 */
	pthread_mutex_t mu;
	long bal;
} acct_t;

void acct_init(acct_t *a, int id, long bal)
{
	a->id = id;
	pthread_mutex_init(&a->mu, NULL);
	a->bal = bal;
}

/* from -> to 로 amt 이체. 0 성공 / -1 실패(NULL, 같은 계좌, amt <= 0, 잔액 부족) */
int transfer(acct_t *from, acct_t *to, long amt)
{
	(void)from;
	(void)to;
	(void)amt;
	/* TODO */
	return -1;
}
