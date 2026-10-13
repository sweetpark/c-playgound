/* ══════════════ strx.h ══════════════
 *
 * [필요한 것]
 *  - strlen/memcpy/strcspn -> <string.h> (common.h 가 포함)
 *  - ret_t                 -> common.h
 *
 * [제공하는 것]
 *  - str_copy(dst, dst_sz, src) : 항상 널 종료. 잘리면 RET_TOO_SMALL (잘린 결과는 dst 에 남음)
 *  - str_chomp(s)               : 끝의 \r \n 제거 (fgets 뒤처리, strcspn 방식)
 *
 * [왜 함수인가]
 *  타입 검사가 되고 디버깅이 쉽다. 매크로는 goto/return 이 필요할 때만 쓴다.
 * ═══════════════════════════════════ */
#ifndef STRX_H
#define STRX_H

#include "common.h"

static inline int str_copy(char *dst, size_t dst_sz, const char *src)
{
    size_t len;

    if (dst == NULL || dst_sz == 0 || src == NULL) {
        return RET_INVALID_ARG;
    }

    len = strlen(src);
    if (len >= dst_sz) {                    /* 널 문자 자리까지 있어야 한다 */
        memcpy(dst, src, dst_sz - 1);
        dst[dst_sz - 1] = '\0';
        return RET_TOO_SMALL;
    }

    memcpy(dst, src, len + 1);
    return RET_OK;
}

/* fgets 로 읽은 줄에서 개행 제거. 개행이 없어도(버퍼가 꽉 찬 경우) 안전하다 */
static inline void str_chomp(char *s)
{
    if (s != NULL) {
        s[strcspn(s, "\r\n")] = '\0';
    }
}

#endif /* STRX_H */
