# HINT2 (막혔을 때만)
```
void db_connect_bad(const char *server, const char *user, char *out, size_t out_size)
{
    /* 1. snprintf(out, out_size, "server=%s;user=%s;pw=%s", server, user, "asdf")
     *    ← 패스워드가 소스 상수 */
}

void db_connect_good(const char *server, const char *user, const char *passwd,
                     char *out, size_t out_size)
{
    /* [ ] 1. 매개변수에 passwd 를 **추가**한다
     *        (호출자·설정파일·시크릿 매니저가 책임진다) */

    /* [ ] 2. snprintf(out, out_size, "server=%s;user=%s;pw=%s", server, user, passwd) */
}
```
