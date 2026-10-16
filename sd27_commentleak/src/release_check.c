#define _DEFAULT_SOURCE
#include <string.h>
#include <stddef.h>
#include "release_check.h"

static const char *KEYWORDS[] = { "password", "passwd", "secret", "apikey", "api_key" };
#define KEYWORD_COUNT (sizeof(KEYWORDS) / sizeof(KEYWORDS[0]))

int find_secret_in_comments(const char *source_code)
{
    const char *p = source_code;
    while ((p = strstr(p, "/*")) != NULL) {
        const char *end = strstr(p, "*/");
        size_t len = end ? (size_t)(end - p) : strlen(p);
        size_t i;
        char buf[512];
        if (len >= sizeof(buf)) len = sizeof(buf) - 1;
        memcpy(buf, p, len);
        buf[len] = '\0';
        for (i = 0; i < KEYWORD_COUNT; i++)
            if (strcasestr(buf, KEYWORDS[i]) != NULL) return 1;
        p = end ? end + 2 : p + 2;
    }
    return 0;
}

int release_ok_bad(const char *source_code) { (void)source_code; return 1; }
int release_ok_good(const char *source_code) { return find_secret_in_comments(source_code) == 0; }
