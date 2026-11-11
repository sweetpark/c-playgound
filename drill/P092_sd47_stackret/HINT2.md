# HINT2 (막혔을 때만)
```
char *rpl_bad(void)
{
    /* 1. char p[10]
     * 2. strcpy(p, "SECRET123")
     * 3. return p   ← 함수가 끝나면 사라지는 스택 주소
     *    (gcc 경고는 pragma 로 끈다, §3 의 WARNING 참고) */
}

char *rpl_good(void)
{
    /* [ ] 1. 지역변수
     *        char p[10], char *buf */

    /* [ ] 2. buf = malloc(10)
     *        NULL 이면 → return NULL */

    /* [ ] 3. strcpy(p, "SECRET123") */

    /* [ ] 4. memcpy(buf, p, 10)   ← 힙으로 복사 */

    /* [ ] 5. return buf   (호출자가 free) */
}
```
