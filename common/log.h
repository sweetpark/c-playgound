/* ══════════════ log.h ══════════════ */
#ifndef LOG_H
#define LOG_H
 
#include <stdio.h>
#include <time.h>
 
typedef enum {
    LOG_LV_ERR = 0,
    LOG_LV_WRN = 1,
    LOG_LV_INF = 2,
    LOG_LV_DBG = 3,
} log_level_t;
 
extern log_level_t g_log_level;     /* 런타임에 바꿀 수 있게 전역 */
 
#define LOG_PRINT(_lv, _tag, _fmt, ...)                                  \
    do {                                                                 \
        if ((_lv) <= g_log_level) {                                      \
            time_t _t = time(NULL);                                      \
            struct tm _tm;                                               \
            char _ts[20];                                                \
            localtime_r(&_t, &_tm);                                      \
            strftime(_ts, sizeof(_ts), "%Y-%m-%d %H:%M:%S", &_tm);       \
            fprintf(stderr, "[%s][%s] " _fmt "\n", _ts, _tag,            \
                    ##__VA_ARGS__);                                      \
        }                                                                \
    } while (0)
 
#define LOG_ERR(_fmt, ...)  LOG_PRINT(LOG_LV_ERR, "ERR", _fmt, ##__VA_ARGS__)
#define LOG_WRN(_fmt, ...)  LOG_PRINT(LOG_LV_WRN, "WRN", _fmt, ##__VA_ARGS__)
#define LOG_INF(_fmt, ...)  LOG_PRINT(LOG_LV_INF, "INF", _fmt, ##__VA_ARGS__)
#define LOG_DBG(_fmt, ...)  LOG_PRINT(LOG_LV_DBG, "DBG", _fmt, ##__VA_ARGS__)
 
#endif /* LOG_H */
