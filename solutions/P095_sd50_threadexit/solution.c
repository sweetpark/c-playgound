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
    pthread_t th;
    if (pthread_create(&th, NULL, worker, result) != 0) return -1;
    g_unreaped++;
    pthread_join(th, NULL);            /* 즉시 회수 */
    g_unreaped--;
    return 0;
}
