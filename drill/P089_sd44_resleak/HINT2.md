# HINT2 (막혔을 때만)
```
int process_bad(int simulate_late_failure)
{
    /* 1. env_hd = alloc_handle()       < 0 이면 return -1
     * 2. con_hd = alloc_handle()       < 0 이면 return -1        ← env_hd 를 안 돌려준다
     * 3. simulate_late_failure 이면 return -1   ← 두 핸들 모두 누수
     * 4. free_handle(con_hd), free_handle(env_hd), return 0 */
}

int process_good(int simulate_late_failure)
{
    /* [ ] 1. env_hd = alloc_handle()
     *        < 0 이면 → return -1 */

    /* [ ] 2. con_hd = alloc_handle()
     *        < 0 이면 → free_handle(env_hd), return -1 */

    /* [ ] 3. simulate_late_failure 이면
     *        free_handle(con_hd)
     *        free_handle(env_hd)     ← 할당 역순
     *        return -1 */

    /* [ ] 4. 정상 경로도 같은 순서로 해제
     *        free_handle(con_hd), free_handle(env_hd) */

    /* [ ] 5. return 0 */
}
```
