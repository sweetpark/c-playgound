/* test_alloc.h — 시험 빌드(-DUNIT_TEST)에서만 할당·초기화를 가로챈다 */
#ifndef TEST_ALLOC_H
#define TEST_ALLOC_H

#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

extern int g_alloc_fail_at;     /* -1: 항상 성공, k: k번째 호출을 실패시킴 */
extern int g_alloc_count;

static inline int t_should_fail(void)
{
    int fail = (g_alloc_fail_at >= 0 && g_alloc_count == g_alloc_fail_at);

    g_alloc_count++;
    return fail;                                    /* 이번 호출만 실패 */
}

static inline void *t_calloc(size_t n, size_t sz) { return t_should_fail() ? NULL : calloc(n, sz); }
static inline char  *t_strdup(const char *s)      { return t_should_fail() ? NULL : strdup(s); }
static inline int    t_mutex_init(pthread_mutex_t *m)
{
    return t_should_fail() ? EAGAIN : pthread_mutex_init(m, NULL);
}

#ifdef UNIT_TEST
#  define CALLOC(_n, _sz)  t_calloc((_n), (_sz))
#  define XSTRDUP(_s)      t_strdup(_s)
#  define MUTEX_INIT(_m)   t_mutex_init(_m)
#else
#  define CALLOC(_n, _sz)  calloc((_n), (_sz))
#  define XSTRDUP(_s)      strdup(_s)
#  define MUTEX_INIT(_m)   pthread_mutex_init((_m), NULL)
#endif

#endif /* TEST_ALLOC_H */
