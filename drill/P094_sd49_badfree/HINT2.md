# HINT2 (막혔을 때만)
```
void free_stack_addr_bad(void)
{
    /* 1. char p[10]
     * 2. free(p)   ← 스택 주소 해제
     *    (gcc 경고는 pragma 로 끈다, §3 의 WARNING 참고) */
}

int alloc_and_free_good(void)
{
    /* [ ] 1. char *p = malloc(10)
     *        NULL 이면 → return -1 */

    /* [ ] 2. p[0] = 'x'   (사용) */

    /* [ ] 3. free(p)   ← malloc 으로 받은 포인터만 해제 */

    /* [ ] 4. return 0 */
}
```
