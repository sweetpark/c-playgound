# HINT2 (막혔을 때만)
```
long fac_bad(int n)
{
    /* 1. return n * fac_bad(n - 1)   ← 종료 조건이 없다 → 스택 고갈
     *    (gcc 경고는 pragma 로 끈다, §3 의 WARNING 참고) */
}

int fac_good(int n, long *out)
{
    /* [ ] 1. fac_good_impl(n, depth, &ok) 를 먼저 만든다
     *        depth > MAX_DEPTH (10000)  → *ok = 0, return 0      ← 깊이 제한
     *        n <= 0                     → *ok = 1, return 1      ← 종료 조건(base case)
     *        그 외                      → inner_ok 를 둔 채 fac_good_impl(n - 1, depth + 1, &inner_ok)
     *                                     inner_ok 가 0 이면 *ok = 0, return 0
     *                                     아니면 *ok = 1, return n * r */

    /* [ ] 2. 바깥 함수
     *        r = fac_good_impl(n, 0, &ok)    ← depth 는 전역이 아니라 **인자**
     *        !ok 이면 → return -1 */

    /* [ ] 3. *out = r, return 0 */
}
```
