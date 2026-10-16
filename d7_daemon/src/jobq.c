#include "common.h"
#include "log.h"
#include "guard.h"
#include "jobq.h"

struct jobq_s {                            /* 불투명: 헤더에는 typedef struct jobq_s jobq_t; 만 둔다 */
    job_t          *buf;
    size_t          cap, head, tail, num;
    bool            closed;                 /* ★ 더 이상 안 받는다 */
    pthread_mutex_t lock;
    pthread_cond_t  not_empty;
    int             done_num;               /* wait_workers (아래 폴드) 가 쓴다 */
    pthread_cond_t  all_done;
};

int jobq_push(jobq_t *q, const job_t *job)
{
    int ret = RET_OK;

    CHK_PTR(q); CHK_PTR(job);

    pthread_mutex_lock(&q->lock);
    if (q->closed)            ret = RET_NOT_SUPPORTED;
    else if (q->num == q->cap) ret = RET_TOO_SMALL;      /* 백프레셔: 버린다 */
    else {
        q->buf[q->tail] = *job;
        q->tail = (q->tail + 1) % q->cap;
        q->num++;
        pthread_cond_signal(&q->not_empty);
    }
    pthread_mutex_unlock(&q->lock);
    return ret;
}

/* 큐가 닫히고 비면 RET_NOT_FOUND 를 돌려 워커를 끝낸다 */
int jobq_pop(jobq_t *q, job_t *out)
{
    int ret = RET_OK;

    CHK_PTR(q); CHK_PTR(out);

    pthread_mutex_lock(&q->lock);
    while (q->num == 0 && !q->closed) {
        pthread_cond_wait(&q->not_empty, &q->lock);      /* ★ while 로 감싼다 */
    }
    if (q->num > 0) {
        *out = q->buf[q->head];
        q->head = (q->head + 1) % q->cap;
        q->num--;
    } else {
        ret = RET_NOT_FOUND;                             /* closed && empty */
    }
    pthread_mutex_unlock(&q->lock);
    return ret;
}

void jobq_close(jobq_t *q)
{
    if (q == NULL) return;
    pthread_mutex_lock(&q->lock);
    q->closed = true;
    pthread_cond_broadcast(&q->not_empty);   /* ★ signal 아님. 전원 깨운다 */
    pthread_mutex_unlock(&q->lock);
}

int jobq_create(size_t cap, jobq_t **out)
{
    int     ret = RET_FAIL;
    jobq_t *q   = NULL;
    int     lock_ok = 0, ne_ok = 0;

    CHK_PTR(out);
    *out = NULL;
    if (cap == 0) { LOG_ERR("cap is 0"); return RET_INVALID_ARG; }

    q = calloc(1, sizeof(*q));
    CHK_ALLOC_GOTO(q, ret, CLEANUP);
    q->buf = calloc(cap, sizeof(*q->buf));
    CHK_ALLOC_GOTO(q->buf, ret, CLEANUP);
    q->cap = cap;

    CHK_COND_GOTO(pthread_mutex_init(&q->lock, NULL) == 0, RET_FAIL, ret, CLEANUP);
    lock_ok = 1;
    CHK_COND_GOTO(pthread_cond_init(&q->not_empty, NULL) == 0, RET_FAIL, ret, CLEANUP);
    ne_ok = 1;
    CHK_COND_GOTO(pthread_cond_init(&q->all_done, NULL) == 0, RET_FAIL, ret, CLEANUP);

    *out = q;  q = NULL;                    /* 소유권 이전 */
    ret  = RET_OK;

CLEANUP:
    if (q != NULL) {                        /* 실패 경로: init 한 것만 destroy */
        if (ne_ok)   pthread_cond_destroy(&q->not_empty);
        if (lock_ok) pthread_mutex_destroy(&q->lock);
        SAFE_FREE(q->buf);
        free(q);
    }
    return ret;
}

void jobq_destroy(jobq_t *q)
{
    if (q == NULL) return;
    pthread_cond_destroy(&q->all_done);
    pthread_cond_destroy(&q->not_empty);
    pthread_mutex_destroy(&q->lock);
    SAFE_FREE(q->buf);
    free(q);
}

/* jobq.h */
/*   #include <netinet/in.h>  <pthread.h>  <stdbool.h>  <stddef.h>  <stdint.h>
 *   typedef struct { uint32_t session_id; uint8_t type; uint16_t len; uint8_t data[1400];
 *                    struct sockaddr_in from; } job_t;
 *   typedef struct jobq_s jobq_t;  + 위 다섯 함수 선언 */
