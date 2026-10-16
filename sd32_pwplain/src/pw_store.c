#include <string.h>
#include "pw_store.h"

unsigned toy_hash(const char *s)
{
    unsigned h = 5381;
    while (*s) h = ((h << 5) + h) + (unsigned char)(*s++);
    return h;
}

void save_password_bad(const char *pw, char *stored, size_t size)
{
    strncpy(stored, pw, size - 1);
    stored[size - 1] = '\0';
}

int verify_password_bad(const char *stored, const char *input)
{
    return strcmp(stored, input) == 0;
}

void save_password_good(const char *pw, unsigned *stored) { *stored = toy_hash(pw); }
int verify_password_good(unsigned stored, const char *input) { return toy_hash(input) == stored; }
