#include <stdio.h>
#include "db_conn.h"

void db_connect_bad(const char *server, const char *user, char *out, size_t out_size)
{
    snprintf(out, out_size, "server=%s;user=%s;pw=%s", server, user, "asdf");
}

void db_connect_good(const char *server, const char *user, const char *passwd, char *out, size_t out_size)
{
    snprintf(out, out_size, "server=%s;user=%s;pw=%s", server, user, passwd);
}
