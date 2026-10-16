/*
 * guard.h
 *
 * [필요한 것]  (예: uint32_t -> <stdint.h>)
 *  - 
 *
 * [제공하는 것]
 *  - 
 */
#ifndef GUARD_H
#define GUARD_H

#include "common.h"
#include "log.h"
#include "safe.h"

#define CHK_PTR(_p) \
    do {\
        if ( (_p)  == NULL ) { \
            LOG_ERR("%s is NULL (%s/%d)", #_p, __func__, __LINE__); \
            return INVALID_ARGS; \
        }\
    } while(0)


#define CHK_RANGE(_v, _min, _max) \
    do { \
        if ( (_v) < _min || (_v) > (_max)) {\
            LOG_ERR("%s (%d) out of range [%d, %d] (%s/%d)",#_v, (int) (_v), (int) _min, (int) _max, __func__, __LINE__); \
            return INVALID_ARGS;\
        }\
    }while(0)


#define CHK_RET(_expr)\
    do { \
        int _r = (_expr);\
        if ( (_expr) != RET_OK ) { \
            LOG_ERR("%s failed: %s(%d)", #_expr, ret_str(_r), _r, __func__, __LINE__);\
        }\
        return (_expr);\
    } while(0)


#define CHK_ALLOC(_p) \
    do { \
        if ( (_p) == NULL ) {\
            LOG_ERR(" alloc %s failed: %s(%d)", #_p, __func__, __LINE__);\
            return FAIL_MEM;\
        }\
    }while (0)


#define CHK_ALLOC_GOTO (_p , _rv, _label) \
    do { \
        if ((_p) == NULL) {\
            LOG_ERR("alloc %s failed (%s/%d)", #_p, __func__, __LINE__);\
            (_rv) = FAIL_MEM;\
            goto _label;\
        }\
    }while(0)


#define CHK_PTR_GOTO(_p, _rv, _label) \
    do { \
        if ((_p) == NULL) {\
            LOG_ERR("%s is NULL (%s/%d)" , #_p, __func__, __LINE__);\
            (_rv) = INVALID_ARGS;\
            goto _label;\
        }\
    }while(0)


#define CHK_RET_GOTO(_expr, _label)\
    do { \
        int _r = (_expr);\
        if ( (_expr) != RET_OK ) { \
            LOG_ERR("%s failed: %s(%d)", #_expr, ret_str(_r), _r, __func__, __LINE__);\
            goto _label;\
        }\
    } while(0)

#define CHK_COND_GOTO(_cond, _code, _rv, _label) \
    do { \
        if ( !(_cond) ) {\
            LOG_ERR("cond (%s) failed (%s/%d)", #_cond, __func__, __LINE__);\
            (_rv) = (_code);\
            goto _label;\
        }\
    }while(0)

#endif /* GUARD_H */
