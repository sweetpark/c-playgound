#include <stdio.h>
#include "salted_hash.h"

static unsigned toy_hash(const char *s)
{
    unsigned h = 5381;
    while (*s) h = ((h << 5) + h) + (unsigned char)(*s++);
    return h;
}

unsigned hash_no_salt_bad(const char *pw) { return toy_hash(pw); }

unsigned hash_with_salt_good(const char *pw, unsigned salt)
{
    char buf[128];
    snprintf(buf, sizeof(buf), "%u:%s", salt, pw);
    return toy_hash(buf);
}
