#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

typedef enum {
    RET_OK          =  0,
    RET_FAIL        = -1,
    RET_INVALID_ARG = -2,
    RET_MEM_LOW     = -3,
    RET_IO_ERROR    = -4,
} ret_t;

static inline const char *ret_str(int ret)
{
    switch (ret) {
    case RET_OK:          return "OK";
    case RET_FAIL:        return "FAIL";
    case RET_INVALID_ARG: return "INVALID_ARG";
    case RET_MEM_LOW:     return "MEM_LOW";
    case RET_IO_ERROR:    return "IO_ERROR";
    default:              return "UNKNOWN";
    }
}

#endif
