#include <string.h>
#include "nullderef.h"

const char *lookup_env_like(const char *key)
{
    if (strcmp(key, "HOME") == 0) return "/home/appuser";
    return NULL;                                   /* 미설정 시 NULL 반환 */
}

void copy_env_bad(const char *key, char *out, size_t out_size)
{
    const char *p = lookup_env_like(key);
    strncpy(out, p, out_size - 1);                  /* NULL 검사 없이 역참조 */
    out[out_size - 1] = '\0';
}

int copy_env_good(const char *key, char *out, size_t out_size)
{
    const char *p = lookup_env_like(key);
    if (p == NULL) return -1;                        /* 사용 전 NULL 검사 */
    strncpy(out, p, out_size - 1);
    out[out_size - 1] = '\0';
    return 0;
}
