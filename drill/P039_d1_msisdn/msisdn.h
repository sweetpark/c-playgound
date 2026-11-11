#define MSISDN_LEN_MAX  15

typedef struct {
    char     in_raw[32];                 /* 입력 원문 (널 포함 32) */
    uint16_t def_cc;                     /* 기본 국가코드 1~999 */
} msisdn_req_t;

typedef struct {
    char     e164[MSISDN_LEN_MAX + 1];   /* 숫자만, '+' 없음.  +1 필수 */
    char     nsn [MSISDN_LEN_MAX + 1];
    uint16_t cc;
    uint8_t  len;
} msisdn_rsp_t;

int msisdn_normalize(const msisdn_req_t *in, msisdn_rsp_t *out);
