/* ══════════════ guard.h ══════════════ */
#ifndef GUARD_H
#define GUARD_H
 
#include "common.h"
#include "log.h"
 
/* ── 즉시 return 하는 가드 (자원을 아직 안 잡았을 때) ── */
 
#define CHK_PTR(_p)                                                     \
    do {                                                                \
        if ((_p) == NULL) {                                             \
            LOG_ERR("%s is NULL (%s/%d)", #_p, __func__, __LINE__);     \
            return RET_INVALID_ARG;                                     \
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
 
/* ── goto 로 빠지는 가드 (자원을 이미 잡았을 때) ── */
 
#define CHK_PTR_GOTO(_p, _label)                                        \
    do {                                                                \
        if ((_p) == NULL) {                                             \
            LOG_ERR("%s is NULL (%s/%d)", #_p, __func__, __LINE__);     \
            ret = RET_INVALID_ARG;                                      \
            goto _label;                                                \
        }                                                               \
    } while (0)
 
#define CHK_RET_GOTO(_expr, _label)                                     \
    do {                                                                \
        ret = (_expr);                                                  \
        if (ret != RET_OK) {                                            \
            LOG_ERR("%s failed: %s(%d) (%s/%d)",                        \
                    #_expr, ret_str(ret), ret, __func__, __LINE__);     \
            goto _label;                                                \
        }                                                               \
    } while (0)
 
#define CHK_COND_GOTO(_cond, _ret, _label)                              \
    do {                                                                \
        if (!(_cond)) {                                                 \
            LOG_ERR("cond(%s) failed (%s/%d)", #_cond, __func__, __LINE__); \
            ret = (_ret);                                               \
            goto _label;                                                \
        }                                                               \
    } while (0)
 
#endif /* GUARD_H */
