#ifndef JOBQ_H
#define JOBQ_H
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "common.h"

typedef struct {
    uint32_t           session_id;
    uint8_t            type;
    uint16_t           len;
    uint8_t            data[1400];
    struct sockaddr_in from;
} job_t;

typedef struct jobq_s jobq_t;

int  jobq_create(size_t cap, jobq_t **out);
int  jobq_push(jobq_t *q, const job_t *job);
int  jobq_pop(jobq_t *q, job_t *out);
void jobq_close(jobq_t *q);
void jobq_destroy(jobq_t *q);
#endif
