#ifndef PW_STORE_H
#define PW_STORE_H
#include <stddef.h>
unsigned toy_hash(const char *s);
void save_password_bad(const char *pw, char *stored, size_t size);
int verify_password_bad(const char *stored, const char *input);
void save_password_good(const char *pw, unsigned *stored);
int verify_password_good(unsigned stored, const char *input);
#endif
