# HINT2 (막혔을 때만)
```
unsigned int len_bad(const char *s)
{
    /* 1. s == NULL 이면 return (unsigned int)-1   ← 4294967295 (역대 가장 큰 "정상값")
     * 2. return strnlen(s, BUFSIZE - 1) */
}

int within_len_bad(unsigned int idx, const char *s)
{
    /* 1. return idx < len_bad(s) */
}

unsigned int len_good(const char *s)
{
    /* [ ] 1. s == NULL 이면 → return 0   (무부호에 안전한 에러값) */

    /* [ ] 2. return (unsigned int)strnlen(s, BUFSIZE - 1)
     *        BUFSIZE 256 */
}

int within_len_good(unsigned int idx, const char *s)
{
    /* [ ] 3. return idx < len_good(s)   ← Bad 와 **같은 식**, 바뀐 건 len 의 에러값 */
}
```
