# HINT2 (막혔을 때만)
```
int spawn_worker_bad(int *result)
{
    /* 1. pthread_create(&th, NULL, worker, result)   실패 → return -1
     * 2. g_unreaped++
     * 3. return 0   ← join 도 detach 도 안 한다 */
}

int spawn_worker_good(int *result)
{
    /* [ ] 1. worker 와 카운터를 먼저 만든다
     *        worker(arg): *(int *)arg = 42, return NULL
     *        static int g_unreaped = 0,  unreaped_count() / reset_unreaped() */

    /* [ ] 2. pthread_create(&th, NULL, worker, result)
     *        실패 → return -1 */

    /* [ ] 3. g_unreaped++ */

    /* [ ] 4. pthread_join(th, NULL)   ← 즉시 회수 */

    /* [ ] 5. g_unreaped-- */

    /* [ ] 6. return 0 */
}
```
