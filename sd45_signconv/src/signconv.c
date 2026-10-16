#define _DEFAULT_SOURCE
#include <string.h>
#include "signconv.h"

#define BUFSIZE 256

unsigned int len_bad(const char *s)
{
    if (s == NULL) return (unsigned int)-1;   /* -1 -> 4294967295 */
    return (unsigned int)strnlen(s, BUFSIZE - 1);
}

unsigned int len_good(const char *s)
{
    if (s == NULL) return 0;                   /* 무부호에 안전한 0 반환 */
    return (unsigned int)strnlen(s, BUFSIZE - 1);
}

int within_len_bad(unsigned int idx, const char *s)
{
    return idx < len_bad(s);
}

int within_len_good(unsigned int idx, const char *s)
{
    return idx < len_good(s);
}
