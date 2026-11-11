#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* P002 sizeof_macros — 모범답안 */
#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))
#define COPY_STR(dst, src) ((void)snprintf((dst), sizeof(dst), "%s", (src)))
#define ZERO_ARRAY(a) ((void)memset((a), 0, sizeof(a)))
