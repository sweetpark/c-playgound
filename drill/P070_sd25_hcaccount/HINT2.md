# HINT2 (막혔을 때만)
```
void db_login_bad(const char *passwd, char *out, size_t out_size)
{
    /* 1. snprintf(out, out_size, "user=%s;pw=%s", "root", passwd)
     *    ← 계정명이 소스 상수 */
}

void db_login_good(const char *user, const char *passwd, char *out, size_t out_size)
{
    /* [ ] 1. 매개변수에 user 를 **추가**한다 */

    /* [ ] 2. snprintf(out, out_size, "user=%s;pw=%s", user, passwd) */
}
```
