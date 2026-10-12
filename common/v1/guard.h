/*
 * guard.h
 *
 * [필요한 것]
 *  - NULL, free, fclose -> <stdlib.h>, <stdio.h> (common.h 가 포함)
 *  - close              -> <unistd.h>
 *  - ret_t, ret_str     -> common.h
 *  - LOG_ERR            -> log.h
 *  - *_GOTO 계열: 호출 함수에 결과를 담을 ret_t 변수(_rv)가 선언돼 있어야 한다
 *
 * [제공하는 것]
 *  - CHK_PTR / CHK_RANGE / CHK_RET           : 실패 시 즉시 return
 *  - CHK_PTR_GOTO / CHK_RET_GOTO / CHK_COND_GOTO : _rv 세팅 후 goto
 *  - SAFE_FREE / SAFE_FCLOSE / SAFE_CLOSE    : 해제 후 NULL(-1) 로 되돌림
 */
#ifndef GUARD_H
#define GUARD_H

#include "common.h"
#include "log.h"
#include <unistd.h>


#define CHK_PTR(_p) \
    do { \
        if((_p) == NULL) {\
            LOG_ERR("%s is NULL (%s/%d)", #_p, __func__, __LINE__);\
            return RET_INVALID_ARG;\
        }\
    }while(0)

#define CHK_RANGE(_v, _min, _max)\
    do { \
        if((_v) < (_min) || (_v) > (_max)){\
            LOG_ERR("%s[%d] out of range [%d, %d] (%s/%d)", #_v, (int)(_v), (int)(_min), (int)(_max), __func__, __LINE__);\
            return RET_INVALID_ARG;\
        }\
    }while(0)


#define CHK_RET(_expr)\
    do{\
        int _r = (_expr);\
        if(_r != RET_OK) {\
            LOG_ERR("%s failed : %s(%d) (%s/%d)", #_expr, ret_str(_r), _r, __func__, __LINE__);\
            return _r;\
        }\
    }while(0)

#define CHK_PTR_GOTO(_p,_rv, _label) \
        do { \
            if((_p) == NULL ){\
                LOG_ERR("%s is NULL (%s/%d)", #_p, __func__, __LINE__);\
                (_rv) = RET_INVALID_ARG;\
                goto _label;\
            }\
        }while(0)

#define CHK_RET_GOTO(_expr, _rv, _label)\
    do {\
        (_rv) = (_expr);\
        if ((_rv) != RET_OK) { \
            LOG_ERR("%s failed : %s(%d) (%s/%d)", #_expr, ret_str(_rv), (_rv), __func__, __LINE__);\
            goto _label;\
        }\
    }while(0)

#define CHK_COND_GOTO(_cond, _code, _rv, _label)\
    do {\
        if(!(_cond)){\
            (_rv) = (_code);\
            LOG_ERR("cond(%s) failed : %s(%d) (%s/%d)", #_cond, ret_str(_rv), (_rv), __func__, __LINE__);\
            goto _label;\
        }\
    }while(0)

#define SAFE_FREE(_p) \
        do {\
            if((_p) != NULL) {\
                free(_p);\
                (_p) = NULL;\
            }\
        }while(0)


#define SAFE_FCLOSE(_fp) \
    do { \
        if((_fp) != NULL) { \
            fclose(_fp);\
            (_fp) = NULL; \
        }\
    }while(0)
 
#define SAFE_CLOSE(_fd) \
    do { \
        if ((_fd)  >= 0 ) {\
            close((_fd));\
            (_fd) = -1;\
        }\
    } while(0)




#endif /* GUARD_H */
