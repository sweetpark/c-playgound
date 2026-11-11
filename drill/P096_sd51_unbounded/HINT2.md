# HINT2 (막혔을 때만)
```
int handle_connection_bad(void)
{
    /* 1. g_active++   ← 상한 없이 생성
     * 2. return 0 */
}

int handle_connection_good(void)
{
    /* [ ] 1. 상수와 카운터
     *        POOL_MAX 4,  static int g_active = 0 */

    /* [ ] 2. 상한 검사 (증가 전)
     *        g_active >= POOL_MAX → return -1   (서비스는 계속 동작) */

    /* [ ] 3. g_active++ */

    /* [ ] 4. return 0 */
}
```
