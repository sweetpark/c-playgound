/* P044 jobq — 요구사항은 problem.md. 헤더(*.h)의 선언을 보고 함수를 채운다. */
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
	(void)q;
	(void)job;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

/* 큐가 닫히고 비면 RET_NOT_FOUND 를 돌려 워커를 끝낸다 */
int jobq_pop(jobq_t *q, job_t *out)
{
	(void)q;
	(void)out;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

void jobq_close(jobq_t *q)
{
	(void)q;
	/* TODO: problem.md 를 보고 Good 구현 */
	drill_todo_hit = 1; /* 구현하면 이 줄을 지운다 */
}

int jobq_create(size_t cap, jobq_t **out)
{
	(void)cap;
	(void)out;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

void jobq_destroy(jobq_t *q)
{
	(void)q;
	/* TODO: problem.md 를 보고 Good 구현 */
	drill_todo_hit = 1; /* 구현하면 이 줄을 지운다 */
}

/* jobq.h */
/*   #include <netinet/in.h>  <pthread.h>  <stdbool.h>  <stddef.h>  <stdint.h>
 *   typedef struct { uint32_t session_id; uint8_t type; uint16_t len; uint8_t data[1400];
 *                    struct sockaddr_in from; } job_t;
 *   typedef struct jobq_s jobq_t;  + 위 다섯 함수 선언 */
