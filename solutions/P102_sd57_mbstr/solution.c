#include <string.h>
#include "mbstr.h"

void mb_copy_bad(char *dst, const char *src)
{
    strcpy(dst, src);                  /* 크기 인자 자체가 없음 -- _mbscpy와 동일한 결함 */
}

int mb_copy_good(char *dst, size_t dst_size, const char *src)
{
    size_t len = strlen(src);
    size_t copy_len;

    if (dst_size == 0) return -1;
    copy_len = (len < dst_size - 1) ? len : dst_size - 1;

    while (copy_len > 0 && ((unsigned char)src[copy_len] & 0xC0) == 0x80) {
        copy_len--;
    }

    memcpy(dst, src, copy_len);
    dst[copy_len] = '\0';
    return 0;
}
