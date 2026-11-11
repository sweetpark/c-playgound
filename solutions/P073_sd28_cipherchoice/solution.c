#include <string.h>
#include <stddef.h>
#include "cipher_check.h"

int is_allowed_cipher_bad(const char *cipher_name)
{
    (void)cipher_name;
    return 1;
}

static const char *BANNED[]  = { "DES-ECB", "3DES", "RC4", "MD5", "SHA1" };
static const char *ALLOWED[] = { "AES-128-CBC", "AES-256-GCM", "ARIA-128-CBC", "SEED-CBC" };

int is_allowed_cipher_good(const char *cipher_name)
{
    size_t i;
    for (i = 0; i < sizeof(BANNED) / sizeof(BANNED[0]); i++)
        if (strcmp(cipher_name, BANNED[i]) == 0) return 0;
    for (i = 0; i < sizeof(ALLOWED) / sizeof(ALLOWED[0]); i++)
        if (strcmp(cipher_name, ALLOWED[i]) == 0) return 1;
    return 0;
}
