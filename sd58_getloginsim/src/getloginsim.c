#include <stdio.h>
#include "getloginsim.h"

static char g_shared_buf[64];

const char *getlogin_like_bad(const char *name)
{
    snprintf(g_shared_buf, sizeof(g_shared_buf), "%s", name);
    return g_shared_buf;              /* 스레드 간 공유되는 정적 버퍼 */
}

int getlogin_like_good_r(const char *name, char *buf, size_t buf_size)
{
    if (snprintf(buf, buf_size, "%s", name) >= (int)buf_size) return -1;
    return 0;
}
