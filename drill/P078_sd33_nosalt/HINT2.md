# HINT2 (막혔을 때만)
```
unsigned hash_no_salt_bad(const char *pw)
{
    /* 1. return toy_hash(pw)   ← 같은 패스워드는 항상 같은 해시 */
}

unsigned hash_with_salt_good(const char *pw, unsigned salt)
{
    /* [ ] 1. 지역변수
     *        char buf[128] */

    /* [ ] 2. 솔트를 앞에 붙여 문자열을 만든다
     *        snprintf(buf, sizeof(buf), "%u:%s", salt, pw) */

    /* [ ] 3. return toy_hash(buf)   ← 해시 함수는 그대로 */
}
```
