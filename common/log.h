/* ══════════════ log.h ══════════════
 *
 * [필요한 것]
 *  - localtime_r -> POSIX. -std=c11 이면 -D_POSIX_C_SOURCE=200809L 필요
 *  - 전역 변수 g_log_level 을 main.c 에서 한 번 정의해야 한다
 *      log_level_t g_log_level = LOG_LV_INF;
 *
 * [제공하는 것]
 *  - LOG_ERR / LOG_WRN / LOG_INF / LOG_DBG : printf 형식 로그 (stderr)
 *  - LOG_ERRNO                             : 메시지 뒤에 strerror(errno) 를 붙임
 * ═══════════════════════════════════ */
#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
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

/* errno 를 먼저 저장한다 — LOG_PRINT 안의 time/localtime_r 이 errno 를 바꿀 수 있다 */
#define LOG_ERRNO(_fmt, ...)                                             \
    do {                                                                 \
        int _e = errno;                                                  \
        LOG_ERR(_fmt ": %s(%d)", ##__VA_ARGS__, strerror(_e), _e);       \
    } while (0)

#endif /* LOG_H */
