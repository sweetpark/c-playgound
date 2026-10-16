/*
 * safe.h
 *
 * [필요한 것]  (예: uint32_t -> <stdint.h>)
 *  - 
 *
 * [제공하는 것]
 *  - 
 */
#ifndef SAFE_H
#define SAFE_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "common.h"


#define SAFE_FREE(_p) \
    do { \
        if((_p) != NULL) {\
            free(_p);\
            (_p) = NULL; \
        }\
       }while (0)

#define SAFE_FCLOSE(_fp) \
    do { \
        if ((_fp) != NULL) { \
            fclose((_fp)); \
            (_fp) = NULL;\
        }\
    }while(0)

#define SAFE_CLOSE(_fd) \
    do { \
        if ( (_fd) >= 0 ) { \
            close((_fd)); \
            (_fd) = -1; \
        }\
    } while(0)

#define SAFE_FCLOSE_RET(_fp, _rv)\
    do { \
        if ( (_fp) != NULL ) { \
            if ( fclose((_fp)) != 0 && (_rv) == RET_OK) { \
                (_rv) = RET_IO_ERROR; \
            } \
            (_fp) = NULL; \
        } \
    }while (0)






#endif /* SAFE_H */
