/* P054 sysconfig — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <stdlib.h>
#include "service_port.h"

int resolve_service_port_bad(const char *portStr)
{
    return atoi(portStr);
}

  /* DEFAULT, ALT1, ALT2 */
#define PORT_TABLE_SIZE (sizeof(PORT_TABLE)/sizeof(PORT_TABLE[0]))

int resolve_service_port_good(int index)
{
	(void)index;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

