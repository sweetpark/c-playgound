/* ══════════════ guard.h ══════════════
 *
 * [필요한 것]
 *  - ret_t, ret_str -> common.h
 *  - LOG_ERR        -> log.h
 *  - *_GOTO 계열: 호출 함수에 결과를 담을 ret_t 변수(_rv)가 선언돼 있어야 한다
 *
 * [제공하는 것]
 *  - 즉시 return : CHK_PTR / CHK_ALLOC / CHK_RANGE / CHK_STR / CHK_RET
 *  - goto        : CHK_PTR_GOTO / CHK_ALLOC_GOTO / CHK_RANGE_GOTO / CHK_STR_GOTO
 *                  CHK_RET_GOTO / CHK_COND_GOTO
 *  - safe.h 를 같이 include 한다 (SAFE_FREE / SAFE_FCLOSE / SAFE_CLOSE ...)
 * ═══════════════════════════════════ */
#ifndef GUARD_H
#define GUARD_H

#include "common.h"
#include "log.h"
#include "safe.h"

/* ── 즉시 return 하는 가드 (자원을 아직 안 잡았을 때) ── */

#define CHK_PTR(_p)                                                     \
    do {                                                                \
        if ((_p) == NULL) {                                             \
            LOG_ERR("%s is NULL (%s/%d)", #_p, __func__, __LINE__);     \
            return RET_INVALID_ARG;                                     \
        }                                                               \
    } while (0)

/* 할당 결과 검사: 인자 오류(INVALID_ARG)가 아니라 NO_MEM 으로 구분한다 */
#define CHK_ALLOC(_p)                                                   \
    do {                                                                \
        if ((_p) == NULL) {                                             \
            LOG_ERR("alloc %s failed (%s/%d)", #_p, __func__, __LINE__);\
            return RET_NO_MEM;                                          \
        }                                                               \
    } while (0)

#define CHK_RANGE(_v, _min, _max)                                       \
    do {                                                                \
        if ((_v) < (_min) || (_v) > (_max)) {                           \
            LOG_ERR("%s(%d) out of range [%d,%d] (%s/%d)",              \
                    #_v, (int)(_v), (int)(_min), (int)(_max),           \
                    __func__, __LINE__);                                \
            return RET_INVALID_ARG;                                     \
        }                                                               \
    } while (0)

#define CHK_STR(_s)                                                     \
    do {                                                                \
        if ((_s) == NULL || (_s)[0] == '\0') {                          \
            LOG_ERR("%s is empty (%s/%d)", #_s, __func__, __LINE__);    \
            return RET_INVALID_ARG;                                     \
        }                                                               \
    } while (0)

#define CHK_RET(_expr)                                                  \
    do {                                                                \
        int _r = (_expr);                                               \
        if (_r != RET_OK) {                                             \
            LOG_ERR("%s failed: %s(%d) (%s/%d)",                        \
                    #_expr, ret_str(_r), _r, __func__, __LINE__);       \
            return _r;                                                  \
        }                                                               \
    } while (0)

/* ── goto 로 빠지는 가드 (자원을 이미 잡았을 때) ──
 *  _rv    : 결과를 담을 변수 (호출 함수의 ret_t 지역변수. 보통 ret)
 *  _label : 빠져나갈 정리 라벨
 * ──────────────────────────────────────────────────── */

#define CHK_PTR_GOTO(_p, _rv, _label)                                   \
    do {                                                                \
        if ((_p) == NULL) {                                             \
            LOG_ERR("%s is NULL (%s/%d)", #_p, __func__, __LINE__);     \
            (_rv) = RET_INVALID_ARG;                                    \
            goto _label;                                                \
        }                                                               \
    } while (0)

#define CHK_ALLOC_GOTO(_p, _rv, _label)                                 \
    do {                                                                \
        if ((_p) == NULL) {                                             \
            LOG_ERR("alloc %s failed (%s/%d)", #_p, __func__, __LINE__);\
            (_rv) = RET_NO_MEM;                                         \
            goto _label;                                                \
        }                                                               \
    } while (0)

#define CHK_RANGE_GOTO(_v, _min, _max, _rv, _label)                     \
    do {                                                                \
        if ((_v) < (_min) || (_v) > (_max)) {                           \
            LOG_ERR("%s(%d) out of range [%d,%d] (%s/%d)",              \
                    #_v, (int)(_v), (int)(_min), (int)(_max),           \
                    __func__, __LINE__);                                \
            (_rv) = RET_INVALID_ARG;                                    \
            goto _label;                                                \
        }                                                               \
    } while (0)

#define CHK_STR_GOTO(_s, _rv, _label)                                   \
    do {                                                                \
        if ((_s) == NULL || (_s)[0] == '\0') {                          \
            LOG_ERR("%s is empty (%s/%d)", #_s, __func__, __LINE__);    \
            (_rv) = RET_INVALID_ARG;                                    \
            goto _label;                                                \
        }                                                               \
    } while (0)

#define CHK_RET_GOTO(_expr, _rv, _label)                                \
    do {                                                                \
        (_rv) = (_expr);                                                \
        if ((_rv) != RET_OK) {                                          \
            LOG_ERR("%s failed: %s(%d) (%s/%d)",                        \
                    #_expr, ret_str(_rv), (_rv), __func__, __LINE__);   \
            goto _label;                                                \
        }                                                               \
    } while (0)

#define CHK_COND_GOTO(_cond, _code, _rv, _label)                        \
    do {                                                                \
        if (!(_cond)) {                                                 \
            LOG_ERR("cond(%s) failed (%s/%d)", #_cond, __func__, __LINE__); \
            (_rv) = (_code);                                            \
            goto _label;                                                \
        }                                                               \
    } while (0)

#endif /* GUARD_H */
