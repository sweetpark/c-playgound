#include <stdio.h>
#include "db_login.h"

void db_login_bad(const char *passwd, char *out, size_t out_size)
{
    snprintf(out, out_size, "user=%s;pw=%s", "root", passwd);
}

void db_login_good(const char *user, const char *passwd, char *out, size_t out_size)
{
    snprintf(out, out_size, "user=%s;pw=%s", user, passwd);
}
