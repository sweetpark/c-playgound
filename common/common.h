/* ══════════════ common.h — 프로젝트 하나당 한 벌 ══════════════ */
#ifndef COMMON_H
#define COMMON_H
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
 
/* ── 리턴코드 ─────────────────────────────────────────
 *  성공은 0 하나. 실패는 전부 음수.
 *  "if (ret != RET_OK)" 한 줄로 모든 실패를 잡을 수 있다.
 * ──────────────────────────────────────────────────── */
typedef enum {
    RET_OK           =  0,    /* 성공 */
    RET_FAIL         = -1,    /* 분류 안 된 일반 실패 */
    RET_INVALID_ARG  = -2,    /* 인자가 잘못됨 (NULL, 범위 밖) */
    RET_NO_MEM       = -3,    /* malloc 실패 */
    RET_NOT_FOUND    = -4,    /* 찾는 대상이 없음 (에러 아닐 수도) */
    RET_TOO_SMALL    = -5,    /* 버퍼가 모자람 */
    RET_BAD_FORMAT   = -6,    /* 파싱 실패, 규격 위반 */
    RET_TIMEOUT      = -7,    /* 시간 초과 */
    RET_IO_ERROR     = -8,    /* read/write/socket 실패 */
    RET_NOT_SUPPORTED= -9,    /* 아직 구현 안 함 / 규격상 미지원 */
} ret_t;
 
/* 에러코드를 사람이 읽는 문자열로. 로그 찍을 때 필수 */
static inline const char *ret_str(int r)
{
    switch (r) {
    case RET_OK:            return "OK";
    case RET_FAIL:          return "FAIL";
    case RET_INVALID_ARG:   return "INVALID_ARG";
    case RET_NO_MEM:        return "NO_MEM";
    case RET_NOT_FOUND:     return "NOT_FOUND";
    case RET_TOO_SMALL:     return "TOO_SMALL";
    case RET_BAD_FORMAT:    return "BAD_FORMAT";
    case RET_TIMEOUT:       return "TIMEOUT";
    case RET_IO_ERROR:      return "IO_ERROR";
    case RET_NOT_SUPPORTED: return "NOT_SUPPORTED";
    default:                return "UNKNOWN";
    }
}
 
#endif /* COMMON_H */
