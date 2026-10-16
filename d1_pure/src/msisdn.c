#include "common.h"
#include "log.h"
#include "guard.h"
#include "msisdn.h"

static int is_allowed_char(char c)
{
    return (c >= '0' && c <= '9') || c == '-' || c == ' ' || c == '+';
}

int msisdn_normalize(const msisdn_req_t *in, msisdn_rsp_t *out)
{
    char        digits[64];
    char        cc_str[8];
    size_t      n = 0, i = 0, cc_len = 0;
    int         has_plus = 0;
    const char *nsn = NULL;

    CHK_PTR(in);
    CHK_PTR(out);
    CHK_RANGE(in->def_cc, 1, 999);

    memset(out, 0x00, sizeof(*out));
    memset(digits, 0x00, sizeof(digits));

    /* 1) 허용 문자 검사 + 숫자만 추출 */
    for (i = 0; i < sizeof(in->in_raw) && in->in_raw[i] != '\0'; i++) {
        char c = in->in_raw[i];

        if (!is_allowed_char(c)) {
            LOG_ERR("bad char 0x%02X at %zu", (unsigned char)c, i);
            return RET_BAD_FORMAT;
        }
        if (c == '+') {
            if (i != 0) {                       /* 맨 앞에만 허용 */
                LOG_ERR("'+' at %zu (must be head)", i);
                return RET_BAD_FORMAT;
            }
            has_plus = 1;
            continue;
        }
        if (c == '-' || c == ' ') continue;     /* 구분자는 무시 */

        if (n >= sizeof(digits) - 1) return RET_BAD_FORMAT;
        digits[n++] = c;
    }
    if (n == 0) { LOG_ERR("no digit"); return RET_BAD_FORMAT; }

    /* 2) 국가코드 분리 */
    cc_len = (size_t)snprintf(cc_str, sizeof(cc_str), "%u", in->def_cc);

    if (has_plus) {
        if (strncmp(digits, cc_str, cc_len) != 0) {
            LOG_ERR("foreign cc: %s (def=%s)", digits, cc_str);
            return RET_NOT_SUPPORTED;           /* 단일 국가 서비스 */
        }
        nsn = digits + cc_len;
    } else if (digits[0] == '0') {
        nsn = digits + 1;                       /* 선행 0 제거 */
    } else if (n > cc_len && strncmp(digits, cc_str, cc_len) == 0) {
        nsn = digits + cc_len;                  /* 이미 82... 로 시작 */
    } else {
        nsn = digits;
    }
    if (nsn[0] == '\0') { LOG_ERR("empty nsn"); return RET_BAD_FORMAT; }

    /* 3) 조립 + 길이 검증 */
    if (cc_len + strlen(nsn) > MSISDN_LEN_MAX) {
        LOG_ERR("e164 too long: %zu", cc_len + strlen(nsn));
        return RET_BAD_FORMAT;
    }

    out->cc  = in->def_cc;
    out->len = (uint8_t)(cc_len + strlen(nsn));
    memcpy(out->nsn,  nsn,    strlen(nsn));   /* 길이는 위에서 검증했고, out 은 memset 0 → 널 종단 */
    memcpy(out->e164, cc_str, cc_len);
    memcpy(out->e164 + cc_len, nsn, strlen(nsn));

    return RET_OK;
}
