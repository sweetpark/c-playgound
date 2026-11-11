# HINT2 (막혔을 때만)
```
int process_request_bad(int step1_will_fail, int *out_balance)
{
    /* 1. rc = step1_will_fail ? -1 : 0
     * 2. rc != 0 이면 { /* do nothing */ }   ← 검사만 하고 조치가 없다
     * 3. *out_balance -= 100                 ← 실패해도 그대로 진행
     * 4. return 0                            ← 실패를 숨긴다 */
}

int process_request_good(int step1_will_fail, int *out_balance)
{
    /* [ ] 1. rc = step1_will_fail ? -1 : 0 */

    /* [ ] 2. rc != 0 이면 → return -1
     *        (실제 조치: 즉시 중단하고 실패를 알린다) */

    /* [ ] 3. *out_balance -= 100 */

    /* [ ] 4. return 0 */
}
```
