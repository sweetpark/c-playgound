#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/random.h>
#include "token_gen.h"

void gen_token_bad(unsigned seed, char *out, size_t out_size)
{
    size_t i;
    srand(seed);
    for (i = 0; i + 1 < out_size; i++)
        out[i] = "0123456789abcdef"[rand() % 16];
    out[i] = '\0';
}

int gen_token_good(char *out, size_t out_size)
{
    unsigned char raw[16];
    size_t i;
    if (out_size < sizeof(raw) * 2 + 1) return -1;
    if (getrandom(raw, sizeof(raw), 0) != (ssize_t)sizeof(raw)) return -1;
    for (i = 0; i < sizeof(raw); i++)
        sprintf(out + i * 2, "%02x", raw[i]);
    return 0;
}
