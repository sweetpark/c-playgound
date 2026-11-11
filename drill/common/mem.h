/* ══════════════ mem.h ══════════════
 *
 * [필요한 것]
 *  - calloc/realloc -> <stdlib.h> (common.h 가 포함)
 *  - LOG_ERR, ret_t -> log.h, common.h
 *
 * [제공하는 것]
 *  - MUL_OVERFLOW(a,b)  : size_t 곱셈 오버플로 검사
 *  - XNEW(p)            : p = calloc(1, sizeof(*p))           (타입 크기 실수 방지)
 *  - XNEWN(p, n)        : p = calloc(n, sizeof(*p))           (곱셈 오버플로 검사 포함)
 *  - XREALLOC(p,sz,rv)  : 임시 포인터로 realloc, 실패해도 원본 p 유지
 *
 * 실패하면 p 는 NULL, 위치 로그를 남긴다. 호출 뒤에는 CHK_ALLOC / CHK_ALLOC_GOTO 로 검사.
 *      XNEW(buf);
 *      CHK_ALLOC_GOTO(buf, ret, CLEANUP);
 * ═══════════════════════════════════ */
#ifndef MEM_H
#define MEM_H

#include "common.h"
#include "log.h"

/* b 가 0 이면 오버플로 아님. 인자는 단순 변수/식 (두 번 평가됨) */
#define MUL_OVERFLOW(_a, _b)                                            \
    ((_b) != 0 && (size_t)(_a) > SIZE_MAX / (size_t)(_b))

#define XNEW(_p)                                                        \
    do {                                                                \
        (_p) = calloc(1, sizeof(*(_p)));                                \
        if ((_p) == NULL) {                                             \
            LOG_ERR("XNEW %s failed (%s/%d)", #_p, __func__, __LINE__); \
        }                                                               \
    } while (0)

#define XNEWN(_p, _n)                                                   \
    do {                                                                \
        if (MUL_OVERFLOW((_n), sizeof(*(_p)))) {                        \
            LOG_ERR("XNEWN %s overflow n=%zu (%s/%d)",                  \
                    #_p, (size_t)(_n), __func__, __LINE__);             \
            (_p) = NULL;                                                \
        } else {                                                        \
            (_p) = calloc((size_t)(_n), sizeof(*(_p)));                 \
            if ((_p) == NULL) {                                         \
                LOG_ERR("XNEWN %s failed (%s/%d)",                      \
                        #_p, __func__, __LINE__);                       \
            }                                                           \
        }                                                               \
    } while (0)

/* realloc 반환값을 원래 포인터에 바로 대입하면 실패 시 원본을 잃는다 (누수).
 * 성공이면 _p 갱신 + _rv = RET_OK, 실패면 _p 그대로 + _rv = RET_NO_MEM. */
#define XREALLOC(_p, _sz, _rv)                                          \
    do {                                                                \
        __typeof__(_p) _tmp = realloc((_p), (_sz));                     \
        if (_tmp == NULL && (size_t)(_sz) != 0) {                       \
            LOG_ERR("XREALLOC %s failed (%s/%d)",                       \
                    #_p, __func__, __LINE__);                           \
            (_rv) = RET_NO_MEM;                                         \
        } else {                                                        \
            (_p) = _tmp;                                                \
            (_rv) = RET_OK;                                             \
        }                                                               \
    } while (0)

#endif /* MEM_H */
