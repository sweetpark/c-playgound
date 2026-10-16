#define _DEFAULT_SOURCE
#include <string.h>
#include <sys/random.h>
#include "salt_gen.h"

unsigned get_salt_bad(void)
{
    return 0xC0FFEE;
}

int get_salt_good(unsigned *out_salt)
{
    unsigned char raw[sizeof(unsigned)];
    if (getrandom(raw, sizeof(raw), 0) != (ssize_t)sizeof(raw)) return -1;
    memcpy(out_salt, raw, sizeof(unsigned));
    return 0;
}
