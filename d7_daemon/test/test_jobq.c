#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "log.h"

log_level_t g_log_level = LOG_LV_ERR;

static int g_fail = 0;

#define T_EQ_INT(_got, _exp)                                            \
    do {                                                                \
        long long _g = (long long)(_got), _e = (long long)(_exp);       \
        if (_g != _e) { g_fail++;                                       \
            printf("  X %s:%d  got=%lld exp=%lld\n",                    \
                   __func__, __LINE__, _g, _e); }                       \
    } while (0)

#include <pthread.h>
#include <sched.h>
#include <stdatomic.h>
#include <time.h>
#include "jobq.h"

static void nap_ms(long ms)
{
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };
    nanosleep(&ts, NULL);
}

static void test_fifo(void)
{
    jobq_t *q = NULL;
    job_t   j;
    int     i = 0;

    T_EQ_INT(jobq_create(4, &q), RET_OK);
    for (i = 0; i < 4; i++) {
        memset(&j, 0x00, sizeof(j));  j.session_id = (uint32_t)i;
        T_EQ_INT(jobq_push(q, &j), RET_OK);
    }
    memset(&j, 0x00, sizeof(j));  j.session_id = 99;
    T_EQ_INT(jobq_push(q, &j), RET_TOO_SMALL);            /* 가득 참: 한 칸 넘김 */

    for (i = 0; i < 4; i++) {
        T_EQ_INT(jobq_pop(q, &j), RET_OK);
        T_EQ_INT(j.session_id, i);                        /* 먼저 넣은 것이 먼저 나온다 */
    }

    jobq_close(q);
    T_EQ_INT(jobq_push(q, &j), RET_NOT_SUPPORTED);        /* 닫힌 큐 */
    T_EQ_INT(jobq_pop(q, &j), RET_NOT_FOUND);             /* 닫혔고 비었다 */
    jobq_destroy(q);
}

static void test_drain_and_wrap(void)
{
    jobq_t *q = NULL;
    job_t   j;
    int     round = 0, i = 0;

    T_EQ_INT(jobq_create(3, &q), RET_OK);
    for (round = 0; round < 100; round++) {               /* head / tail 가 몇 바퀴 돈다 */
        for (i = 0; i < 3; i++) {
            memset(&j, 0x00, sizeof(j));  j.session_id = (uint32_t)(round * 3 + i);
            T_EQ_INT(jobq_push(q, &j), RET_OK);
        }
        for (i = 0; i < 3; i++) {
            T_EQ_INT(jobq_pop(q, &j), RET_OK);
            T_EQ_INT(j.session_id, round * 3 + i);
        }
    }

    for (i = 0; i < 3; i++) { j.session_id = (uint32_t)i; jobq_push(q, &j); }
    jobq_close(q);
    for (i = 0; i < 3; i++) T_EQ_INT(jobq_pop(q, &j), RET_OK);   /* ★ 닫혀도 남은 건 꺼낸다 */
    T_EQ_INT(jobq_pop(q, &j), RET_NOT_FOUND);
    jobq_destroy(q);
}

static void test_null(void)
{
    jobq_t *q = NULL;
    job_t   j;

    memset(&j, 0x00, sizeof(j));
    T_EQ_INT(jobq_create(0, &q), RET_INVALID_ARG);
    T_EQ_INT(q == NULL, 1);                               /* ★ 실패하면 NULL */
    T_EQ_INT(jobq_create(4, NULL), RET_INVALID_ARG);
    T_EQ_INT(jobq_push(NULL, &j), RET_INVALID_ARG);
    T_EQ_INT(jobq_pop(NULL, &j), RET_INVALID_ARG);
    jobq_close(NULL);                                     /* 죽지 않으면 통과 */
    jobq_destroy(NULL);
}

/* ── 스레드 시험 ─────────────────────────────────────────────────── */
static atomic_long g_sum;
static atomic_int  g_exited;

static void *consumer(void *arg)
{
    jobq_t *q = (jobq_t *)arg;
    job_t   j;

    while (jobq_pop(q, &j) == RET_OK) atomic_fetch_add(&g_sum, (long)j.session_id);
    atomic_fetch_add(&g_exited, 1);
    return NULL;
}

static void *producer(void *arg)
{
    jobq_t *q = (jobq_t *)arg;
    job_t   j;
    int     i = 0;

    memset(&j, 0x00, sizeof(j));
    for (i = 1; i <= 5000; i++) {
        j.session_id = (uint32_t)i;
        while (jobq_push(q, &j) == RET_TOO_SMALL) sched_yield();   /* 가득 차면 잠깐 양보 */
    }
    return NULL;
}

static void test_threads(void)
{
    jobq_t   *q = NULL;
    pthread_t c[4], p[2];
    int       i = 0;

    atomic_store(&g_sum, 0);
    T_EQ_INT(jobq_create(16, &q), RET_OK);
    for (i = 0; i < 4; i++) pthread_create(&c[i], NULL, consumer, q);
    for (i = 0; i < 2; i++) pthread_create(&p[i], NULL, producer, q);
    for (i = 0; i < 2; i++) pthread_join(p[i], NULL);
    jobq_close(q);                                        /* 생산이 끝난 뒤에 닫는다 */
    for (i = 0; i < 4; i++) pthread_join(c[i], NULL);
    T_EQ_INT(atomic_load(&g_sum), 2L * (5000L * 5001L / 2L));   /* ★ 유실도 중복도 없다 */
    jobq_destroy(q);
}

static void test_close_wakes_all(void)
{
    jobq_t   *q = NULL;
    pthread_t c[4];
    int       i = 0, waited = 0;

    atomic_store(&g_exited, 0);
    T_EQ_INT(jobq_create(4, &q), RET_OK);
    for (i = 0; i < 4; i++) pthread_create(&c[i], NULL, consumer, q);   /* 전부 빈 큐에서 대기 */
    nap_ms(100);
    T_EQ_INT(atomic_load(&g_exited), 0);

    jobq_close(q);
    for (waited = 0; waited < 2000 && atomic_load(&g_exited) < 4; waited += 10) nap_ms(10);
    T_EQ_INT(atomic_load(&g_exited), 4);                  /* ★ broadcast 가 아니면 1개만 깬다 */
    if (atomic_load(&g_exited) < 4) { printf("  X hang: skip join\n"); g_fail++; return; }

    for (i = 0; i < 4; i++) pthread_join(c[i], NULL);
    jobq_destroy(q);
}

int main(void)
{
    test_fifo();
    test_drain_and_wrap();
    test_null();
    test_threads();
    test_close_wakes_all();

    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
