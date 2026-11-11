# HINT2 (막혔을 때만)
```
int bind_socket_bad(int port)
{
    /* 1. fd = socket(AF_INET, SOCK_STREAM, 0)
     * 2. setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on))   ← 습관적으로 켠 옵션
     * 3. addr = { AF_INET, htonl(INADDR_ANY), htons(port) }           ← 모든 인터페이스
     * 4. bind 실패 → close(fd), return -1
     * 5. return fd */
}

int bind_socket_good(int port, const char *bind_ip)
{
    /* [ ] 1. fd = socket(AF_INET, SOCK_STREAM, 0)
     *        fd < 0 이면 → return -1 */

    /* [ ] 2. SO_REUSEPORT 를 **켜지 않는다**
     *        (부하 분산이 실제로 필요할 때만 켠다) */

    /* [ ] 3. addr 을 0 으로 채우고 family, port 설정
     *        inet_pton(AF_INET, bind_ip, &addr.sin_addr)   ← 서비스할 인터페이스로 범위 축소 */

    /* [ ] 4. bind 실패 → close(fd), return -1 */

    /* [ ] 5. return fd   (호출자가 close) */
}
```
