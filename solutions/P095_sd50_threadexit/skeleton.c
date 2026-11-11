/* P095 threadexit — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <pthread.h>
#include "threadexit.h"

static int g_unreaped = 0;

static void *worker(void *arg)
{
    int *r = (int *)arg;
    *r = 42;
    return NULL;
}

int unreaped_count(void) { return g_unreaped; }
void reset_unreaped(void) { g_unreaped = 0; }

int spawn_worker_bad(int *result)
{
    pthread_t th;
    if (pthread_create(&th, NULL, worker, result) != 0) return -1;
    g_unreaped++;                      /* join도 detach도 안 함 -> 스레드 자원이 회수되지 않음 */
    return 0;
}

int spawn_worker_good(int *result)
{
	(void)result;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

