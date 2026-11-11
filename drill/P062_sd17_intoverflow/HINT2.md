# HINT2 (막혔을 때만)
```
void *int_alloc_bad(int count)
{
    /* 1. int total = count + RESERVE   (RESERVE 8)
     * 2. return malloc((size_t)total * sizeof(int))   ← 오버플로우된 값을 그대로 */
}

void use_buffer_bad(int count)
{
    /* 1. buf = int_alloc_bad(count)
     * 2. buf[0] = 42   ← 반환값 검사 없음
     * 3. free(buf) */
}

void *int_alloc_good(int count)
{
    /* [ ] 1. 덧셈 전 검사
     *        count < 0 또는 count > INT_MAX - RESERVE → return NULL */

    /* [ ] 2. int total = count + RESERVE */

    /* [ ] 3. 곱셈 전 검사
     *        (size_t)total > SIZE_MAX / sizeof(int) → return NULL */

    /* [ ] 4. return malloc((size_t)total * sizeof(int)) */
}

void *use_buffer_good(int count)
{
    /* [ ] 1. buf = int_alloc_good(count)
     *        NULL 이면 → return NULL */

    /* [ ] 2. buf[0] = 42 */

    /* [ ] 3. return buf   (해제는 호출자) */
}
```
