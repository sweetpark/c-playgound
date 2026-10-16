#ifndef SQL_AUTH_H
#define SQL_AUTH_H
int login_bad(const char *userId, const char *password);
int login_good(const char *userId, const char *password);
#endif
