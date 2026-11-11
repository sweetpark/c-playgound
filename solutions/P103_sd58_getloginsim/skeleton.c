/* P103 getloginsim — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
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
	(void)name;
	(void)buf;
	(void)buf_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

