/* P081 reuseport — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 
#endif
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
	(void)port;
	(void)bind_ip;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

