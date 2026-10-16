/*
 * log.h
 *
 * [필요한 것]  (예: uint32_t -> <stdint.h>)
 *  - 
 *
 * [제공하는 것]
 *  - 
 */
#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

typedef enum{
    ERR=0,
    WRN,
    INF,
    DBG
} log_level_t;

extern log_level_t g_log_level;

#define LOG_PRINT(_level, _tag, _fmt, ...) \
    do { \
       if ((_level) <= g_log_level) {\
           time_t _t = time(NULL);\
           struct tm _tm;\
           char _ts[20]; \
           localtime_r(&_t, &_tm);\
           strftime(_ts, sizeof(_ts), "%Y-%m-%d %H:%M:%S", &_tm); \
           fprintf(stderr, "[%s][%s] " _fmt "\n", _ts, _tag, ##__VA_ARGS__);\
       } \
    }while (0)


#define LOG_ERR(_fmt, ...) LOG_PRINT(LOG_LV_ERR, "ERR", _fmt, ##__VA_ARGS__)
#define LOG_WRN(_fmt, ...) LOG_PRINT(LOG_LV_WRN, "WRN", _fmt, ##__VA_ARGS__)
#define LOG_INF(_fmt, ...) LOG_PRINT(LOG_LV_INF, "INF", _fmt, ##__VA_ARGS__)
#define LOG_DBG(_fmt, ...) LOG_PRINT(LOG_LV_DBG, "DBG", _fmt, ##__VA_ARGS__)


#define LOG_ERRNO(_fmt, ...) \
    do {\
        int _e = errno;\
        LOG_ERR(_fmt ": %s(%d)", ##__VA_ARGS__, strerror(_e), _e);\
    }while (0)



#endif /* LOG_H */
