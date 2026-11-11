# HINT2 (막혔을 때만)
```
unsigned get_salt_bad(void)
{
    /* 1. return 0xC0FFEE   ← 항상 같은 솔트 */
}

int get_salt_good(unsigned *out_salt)
{
    /* [ ] 1. 지역변수
     *        unsigned char raw[sizeof(unsigned)] */

    /* [ ] 2. getrandom(raw, sizeof(raw), 0) != (ssize_t)sizeof(raw) → return -1 */

    /* [ ] 3. memcpy(out_salt, raw, sizeof(unsigned)) */

    /* [ ] 4. return 0 */
}
```
