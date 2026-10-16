#ifndef DB_CONN_H
#define DB_CONN_H
#include <stddef.h>
void db_connect_bad(const char *server, const char *user, char *out, size_t out_size);
void db_connect_good(const char *server, const char *user, const char *passwd, char *out, size_t out_size);
#endif
