# HINT2 (막혔을 때만)
```
int parse_port_bad(const char *env_val)
{
    /* 1. return atoi(env_val)   ← 범위 검사 없음 */
}

int parse_port_good(const char *env_val)
{
    /* [ ] 1. 상수
     *        PORT_MIN 3000, PORT_MAX 3003, PORT_DEFAULT 3000 */

    /* [ ] 2. int port = atoi(env_val) */

    /* [ ] 3. 범위 검사
     *        port < PORT_MIN 또는 port > PORT_MAX
     *            → return PORT_DEFAULT  (실패 대신 안전한 기본값) */

    /* [ ] 4. return port */
}
```
