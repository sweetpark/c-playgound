# HINT2 (막혔을 때만)
```
void do_work_bad(void)
{
    /* 1. LOCK_BEGIN()
     * 2. 임계 구역 작업
     * 3. (LOCK_END() 를 빠뜨림)   ← 잠금이 영원히 풀리지 않는다 */
}

void do_work_good(void)
{
    /* [ ] 1. 매크로와 카운터
     *        static int g_lock_depth = 0
     *        LOCK_BEGIN() = (g_lock_depth++),  LOCK_END() = (g_lock_depth--) */

    /* [ ] 2. LOCK_BEGIN()
     *        LOCK_END()       ← **먼저 두 줄을 다 쓰고** 그 사이를 채운다 */

    /* [ ] 3. 두 줄 사이에 임계 구역 작업 */
}
```
