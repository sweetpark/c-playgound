/*
 * common.h
 *
 * [필요한 것]  (예: uint32_t -> <stdint.h>)
 *  - 
 *
 * [제공하는 것]
 *  - 
 */
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>



typedef enum {
    OK = 1,
    FAIL = -1,
    INVALID_ARGS = -2,
    FAIL_MEM = -3,
    INVALID_IO = -4,
} ret_t;



const char *ret_str(ret_t code){
    switch(code){
        case OK:
            return "OK";
        case FAIL:
            return "FAIL";
        case INVALID_ARGS:
            return "INVALID_ARGS";
        case FAIL_MEM:
            return "FAIL_MEM";
        case INVALID_IO:
            return "INVALID_IO";
        defualt:
            return "UNKNOWN";
   }
}



#endif /* COMMON_H */
