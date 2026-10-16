#ifndef MSG_H
#define MSG_H
#include <stddef.h>
#include <stdint.h>
#include "be.h"
#define HDR_LEN        12
#define CSUM_LEN        2
#define PAYLOAD_MAX  1400

typedef struct {
    uint8_t  ver;
    uint8_t  type;
    uint8_t  flags;
    uint8_t  prio;
    uint32_t session_id;
    uint32_t seq;
    uint16_t payload_len;
    uint8_t  payload[PAYLOAD_MAX];
} msg_t;

uint16_t inet_csum(const uint8_t *b, size_t len);
int msg_pack  (const msg_t *in, uint8_t *buf, size_t buf_len, size_t *out_len);
int msg_unpack(const uint8_t *buf, size_t buf_len, msg_t *out, size_t *consumed);
#endif
