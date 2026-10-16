#include <stdio.h>
#include <unistd.h>
#include "port_bind.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static int pick_port(void)
{
    return 20000 + (int)(getpid() % 5000);
}

static void test_bad_is_vulnerable(void)
{
    int port = pick_port();
    int fd1 = bind_socket_bad(port);
    int fd2 = bind_socket_bad(port);   /* 같은 UID로 도는 "다른 코드"를 흉내 */

    T_TRUE(fd1 >= 0, "첫 번째 바인딩은 성공해야 한다");
    T_TRUE(fd2 >= 0,
        "Bad는 SO_REUSEPORT 때문에 같은 포트에 두 번째 소켓도 바인딩되어야 한다(가로채기 가능 = 취약점 재현)");

    if (fd1 >= 0) close(fd1);
    if (fd2 >= 0) close(fd2);
}

static void test_good_blocks_second_bind(void)
{
    int port = pick_port() + 1;
    int fd1 = bind_socket_good(port, "127.0.0.1");
    int fd2 = bind_socket_good(port, "127.0.0.1");   /* 같은 코드가 실수로 두 번 뜬 상황 */

    T_TRUE(fd1 >= 0, "첫 번째 바인딩은 성공해야 한다");
    T_TRUE(fd2 == -1,
        "Good은 SO_REUSEPORT가 없으므로 같은 포트의 두 번째 바인딩이 실패해야 한다(EADDRINUSE)");

    if (fd1 >= 0) close(fd1);
    if (fd2 >= 0) close(fd2);
}

int main(void)
{
    test_bad_is_vulnerable();
    test_good_blocks_second_bind();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
