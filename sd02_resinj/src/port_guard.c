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
    int port = atoi(env_val);
    if (port < PORT_MIN || port > PORT_MAX) return PORT_DEFAULT;
    return port;
}
