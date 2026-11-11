#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>

/* P027 run_squares — 모범답안 */
#define MAX_WORKERS 64

typedef struct {
	int i;          /* 이 스레드 전용 입력 (호출 함수의 지역 변수 주소를 넘기지 않는다) */
	long *slot;     /* 결과를 쓸 칸 */
} job_t;

static void *square_worker(void *arg)
{
	job_t *j = arg;
	*j->slot = (long)j->i * j->i;
	return NULL;
}

int run_squares(int n, long *out)
{
	job_t jobs[MAX_WORKERS];            /* join 할 때까지 살아 있는 배열: 스레드마다 자기 칸 */
	pthread_t th[MAX_WORKERS];
	int started = 0;
	int i, rc = 0;

	if (n <= 0 || n > MAX_WORKERS || out == NULL)
		return -1;

	for (i = 0; i < n; i++) {
		jobs[i].i = i;
		jobs[i].slot = &out[i];
		if (pthread_create(&th[i], NULL, square_worker, &jobs[i]) != 0) {
			rc = -1;
			break;
		}
		started++;
	}
	for (i = 0; i < started; i++)       /* 만든 스레드는 반드시 join */
		pthread_join(th[i], NULL);
	return rc;
}
