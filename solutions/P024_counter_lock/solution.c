#include <pthread.h>
#include <stddef.h>

/* P024 counter_lock — 모범답안 */
typedef struct {
	pthread_mutex_t mu;
	long v;
} counter_t;

int counter_init(counter_t *c)
{
	pthread_mutexattr_t attr;
	int rc;

	if (c == NULL)
		return -1;
	if (pthread_mutexattr_init(&attr) != 0)
		return -1;
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
	rc = pthread_mutex_init(&c->mu, &attr);
	pthread_mutexattr_destroy(&attr);
	if (rc != 0)
		return -1;
	c->v = 0;
	return 0;
}

void counter_add(counter_t *c, long d)
{
	pthread_mutex_lock(&c->mu);
	c->v += d;
	pthread_mutex_unlock(&c->mu);       /* 잡은 함수가 푼다 */
}

long counter_get(counter_t *c)
{
	long v;

	pthread_mutex_lock(&c->mu);
	v = c->v;                           /* return 뒤에 unlock 을 쓰지 않도록 값을 담아 둔다 */
	pthread_mutex_unlock(&c->mu);
	return v;
}

int counter_destroy(counter_t *c)
{
	return pthread_mutex_destroy(&c->mu);
}
