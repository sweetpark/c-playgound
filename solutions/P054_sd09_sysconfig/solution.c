#include <stdlib.h>
#include "service_port.h"

int resolve_service_port_bad(const char *portStr)
{
    return atoi(portStr);
}

static const int PORT_TABLE[] = { 21, 2121, 8021 };  /* DEFAULT, ALT1, ALT2 */
#define PORT_TABLE_SIZE (sizeof(PORT_TABLE)/sizeof(PORT_TABLE[0]))

int resolve_service_port_good(int index)
{
    if (index < 0 || (size_t)index >= PORT_TABLE_SIZE) return -1;
    return PORT_TABLE[index];
}
