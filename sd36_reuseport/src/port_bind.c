#define _DEFAULT_SOURCE
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "port_bind.h"

int bind_socket_bad(int port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int on = 1;
    struct sockaddr_in addr;
    if (fd < 0) return -1;

    setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));   /* 습관적으로 켠 옵션 */

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);                    /* 모든 인터페이스 */
    addr.sin_port = htons((unsigned short)port);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        close(fd);
        return -1;
    }
    return fd;
}

int bind_socket_good(int port, const char *bind_ip)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    if (fd < 0) return -1;

    /* SO_REUSEPORT 를 켜지 않는다 -- 부하 분산이 실제로 필요할 때만 켠다 */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, bind_ip, &addr.sin_addr);                 /* 특정 인터페이스로 범위 축소 */
    addr.sin_port = htons((unsigned short)port);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        close(fd);
        return -1;
    }
    return fd;
}
