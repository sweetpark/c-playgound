/* P047 resinj — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <stdlib.h>
#include "port_guard.h"

#define PORT_MIN     3000
#define PORT_MAX     3003
#define PORT_DEFAULT 3000

int parse_port_bad(const char *env_val)
{
    return atoi(env_val);
}

int parse_port_good(const char *env_val)
{
	(void)env_val;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

