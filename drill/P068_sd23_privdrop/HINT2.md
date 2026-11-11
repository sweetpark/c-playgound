# HINT2 (막혔을 때만)
```
void elevate_bad(void)
{
    /* 1. g_current_priv = ROOT_UID   ← 올리고 되돌리지 않는다 */
}

void do_privileged_work_good(void)
{
    /* [ ] 1. 현재 권한을 백업한다
     *        int backup = g_current_priv */

    /* [ ] 2. g_current_priv = ROOT_UID */

    /* [ ] 3. 필요한 작업만 이 구간에서 수행한다 */

    /* [ ] 4. g_current_priv = backup   ← 즉시 복귀 */
}
```
