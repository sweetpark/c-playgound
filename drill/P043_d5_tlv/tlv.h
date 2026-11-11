#ifndef TLV_H
#define TLV_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define IEI_SESSION_ID  0x01
#define IEI_CAUSE       0x02
#define IEI_UE_NAME     0x10
#define IEI_TIMESTAMP   0x20
#define IEI_NESTED      0x30

#define NAME_LEN_MAX      32
#define UNKNOWN_NUM_MAX    8
#define UNKNOWN_VAL_MAX  128

typedef struct {
    uint8_t  iei;
    uint16_t len;
    uint8_t  val[UNKNOWN_VAL_MAX];
} unknown_ie_t;

typedef struct {
    uint32_t     session_id;          /* M */
    uint8_t      cause;               /* M */
    bool         has_session_id;      /*   수신 여부 추적용 */
    bool         has_cause;

    bool         has_name;
    char         name[NAME_LEN_MAX + 1];
    bool         has_ts;
    uint64_t     ts_ms;

    unknown_ie_t unknown[UNKNOWN_NUM_MAX];
    uint8_t      unknown_num;
} msg_body_t;

int tlv_parse(const uint8_t *buf, size_t buf_len, msg_body_t *out);
int tlv_build(const msg_body_t *in, uint8_t *buf, size_t buf_len, size_t *out_len);
#endif
