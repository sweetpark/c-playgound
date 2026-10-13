/* ══════════════ safe.h ══════════════
 *
 * [필요한 것]
 *  - free/fclose/close -> <stdlib.h>, <stdio.h>, <unistd.h>
 *  - RET_IO_ERROR      -> common.h
 *
 * [제공하는 것]
 *  - SAFE_FREE / SAFE_FCLOSE / SAFE_CLOSE : 해제 후 NULL(-1) 로 되돌림
 *  - SAFE_FCLOSE_RET                      : fclose 실패를 _rv 에 반영 (쓰기 파일용)
 *  - SAFE_CPY                             : 배열 dst 전용 snprintf 복사 (포인터면 컴파일 에러)
 *  - IS_ARRAY                             : 배열인지 컴파일 타임 판별
 *
 * [주의]
 *  - 인자는 단순 변수만. SAFE_FREE(p++) 처럼 부작용 있는 식은 금지 (여러 번 평가됨)
 *  - SAFE_CLOSE 는 fd 변수를 -1 로 초기화해 둬야 안전하다
 * ═══════════════════════════════════ */
#ifndef SAFE_H
#define SAFE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"

#define SAFE_FREE(_p)                                                   \
    do {                                                                \
        if ((_p) != NULL) {                                             \
            free(_p);                                                   \
            (_p) = NULL;                                                \
        }                                                               \
    } while (0)

#define SAFE_FCLOSE(_fp)                                                \
    do {                                                                \
        if ((_fp) != NULL) {                                            \
            fclose(_fp);                                                \
            (_fp) = NULL;                                               \
        }                                                               \
    } while (0)

#define SAFE_CLOSE(_fd)                                                 \
    do {                                                                \
        if ((_fd) >= 0) {                                               \
            close(_fd);                                                 \
            (_fd) = -1;                                                 \
        }                                                               \
    } while (0)

/* 쓰기용 파일: 쓰기 오류는 fclose 에서 드러날 수 있다.
 * 이미 실패한 _rv 는 덮어쓰지 않는다 (첫 실패 원인을 보존). */
#define SAFE_FCLOSE_RET(_fp, _rv)                                       \
    do {                                                                \
        if ((_fp) != NULL) {                                            \
            if (fclose(_fp) != 0 && (_rv) == RET_OK) {                  \
                (_rv) = RET_IO_ERROR;                                   \
            }                                                           \
            (_fp) = NULL;                                               \
        }                                                               \
    } while (0)

/* 배열이면 1, 포인터면 0 (GCC/Clang) */
#define IS_ARRAY(_a)                                                    \
    (!__builtin_types_compatible_p(__typeof__(_a), __typeof__(&(_a)[0])))

/* dst 가 포인터면 sizeof(_dst) 가 8 이 되어 조용히 잘리므로, 컴파일 단계에서 막는다.
 * 잘림 여부까지 알아야 하면 strx.h 의 str_copy() 를 쓴다. */
#define SAFE_CPY(_dst, _src)                                            \
    do {                                                                \
        (void)sizeof(char[IS_ARRAY(_dst) ? 1 : -1]);                    \
        snprintf((_dst), sizeof(_dst), "%s", (_src) ? (_src) : "");     \
    } while (0)

#endif /* SAFE_H */
