#ifndef DB_LOGIN_H
#define DB_LOGIN_H
#include <stddef.h>
void db_login_bad(const char *passwd, char *out, size_t out_size);
void db_login_good(const char *user, const char *passwd, char *out, size_t out_size);
#endif
