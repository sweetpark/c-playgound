#include "common.h"
#include "log.h"
#include "guard.h"
#include "be.h"
#include "tlv.h"

static void keep_unknown(msg_body_t *out, uint8_t t, const uint8_t *v, uint32_t l)
{
    unknown_ie_t *u = NULL;

    if (out->unknown_num >= UNKNOWN_NUM_MAX || l > UNKNOWN_VAL_MAX) {
        LOG_WRN("drop unknown IEI=0x%02X len=%u", t, l);   /* 버리되 죽지 않는다 */
        return;
    }
    u = &out->unknown[out->unknown_num++];
    u->iei = t;
    u->len = (uint16_t)l;
    if (l > 0) memcpy(u->val, v, l);
}

/* D5 의 NEED: 입력이 모자란 것은 '규격 위반' 이므로 BAD_FORMAT 이다 (D4 의 NEED 는 TOO_SMALL) */
#define NEED(_n)                                                        \
    do {                                                                \
        if ((size_t)(_n) > buf_len - off) {                             \
            LOG_ERR("need %zu but %zu left (%s/%d)",                    \
                    (size_t)(_n), buf_len - off, __func__, __LINE__);   \
            return RET_BAD_FORMAT;                                      \
        }                                                               \
    } while (0)

#define DUP_CHK(_has, _name)                                            \
    do {                                                                \
        if (_has) { LOG_WRN("dup IE %s, keep first", _name); break; }   \
    } while (0)

/* Nested 를 위해 depth 를 받는다. memset 과 필수 IE 검사는 바깥(tlv_parse)이 한다 */
static int parse_ies(const uint8_t *buf, size_t buf_len, msg_body_t *out, int depth)
{
    size_t off = 0;

    while (off < buf_len) {
        uint8_t  t = 0;
        uint32_t l = 0;

        NEED(2);                                   /* ① T + L */
        t = buf[off++];
        l = buf[off++];

        if (l == 0xFF) {                           /* ② 확장 길이 */
            NEED(2);
            l = rd_u16be(buf + off);
            off += 2;
        }

        NEED(l);                                   /* ③ V 실재 확인 */

        switch (t) {
        case IEI_SESSION_ID:
            if (l != 4) { LOG_ERR("sessionId len=%u", l); return RET_BAD_FORMAT; }
            if (!out->has_session_id) {
                out->session_id     = rd_u32be(buf + off);
                out->has_session_id = true;
            } else DUP_CHK(true, "SessionId");
            break;

        case IEI_CAUSE:
            if (l != 1) return RET_BAD_FORMAT;
            if (buf[off] > 63) { LOG_ERR("cause=%u", buf[off]); return RET_BAD_FORMAT; }
            if (!out->has_cause) { out->cause = buf[off]; out->has_cause = true; }
            break;

        case IEI_UE_NAME:
            if (l > NAME_LEN_MAX) return RET_BAD_FORMAT;
            if (!out->has_name) {
                if (l > 0) memcpy(out->name, buf + off, l);
                out->name[l] = '\0';               /* ★ 널 종단은 내가 붙인다 */
                out->has_name = true;
            }
            break;

        case IEI_TIMESTAMP:
            if (l != 8) return RET_BAD_FORMAT;
            if (!out->has_ts) {
                out->ts_ms = ((uint64_t)rd_u32be(buf + off) << 32)
                           |  (uint64_t)rd_u32be(buf + off + 4);
                out->has_ts = true;
            }
            break;

        case IEI_NESTED:
            /* 깊이 1까지만: Nested 안의 Nested 는 BAD_FORMAT */
            if (depth >= 1) { LOG_ERR("nested depth %d", depth + 1); return RET_BAD_FORMAT; }
            CHK_RET(parse_ies(buf + off, l, out, depth + 1));
            break;

        default:                                   /* ④ 모르는 것은 보존 후 통과 */
            keep_unknown(out, t, buf + off, l);
            break;
        }

        off += l;                                  /* ⑤ 전진은 여기 한 곳 */
    }
    return RET_OK;
}

int tlv_parse(const uint8_t *buf, size_t buf_len, msg_body_t *out)
{
    CHK_PTR(buf);
    CHK_PTR(out);
    memset(out, 0x00, sizeof(*out));

    CHK_RET(parse_ies(buf, buf_len, out, 0));

    /* 필수 IE 검사는 순회가 끝난 뒤에 (순서 보장이 없으므로) */
    if (!out->has_session_id) { LOG_ERR("missing SessionId"); return RET_BAD_FORMAT; }
    if (!out->has_cause)      { LOG_ERR("missing Cause");     return RET_BAD_FORMAT; }

    return RET_OK;
}

#define ROOM(_n)                                                        \
    do {                                                                \
        if ((size_t)(_n) > buf_len - off) return RET_TOO_SMALL;         \
    } while (0)

int tlv_build(const msg_body_t *in, uint8_t *buf, size_t buf_len, size_t *out_len)
{
    size_t off = 0;
    int    i   = 0;

    CHK_PTR(in);
    CHK_PTR(buf);
    CHK_PTR(out_len);
    *out_len = 0;

    ROOM(2 + 4);                                   /* SessionId */
    buf[off++] = IEI_SESSION_ID;  buf[off++] = 4;
    wr_u32be(buf + off, in->session_id);  off += 4;

    ROOM(2 + 1);                                   /* Cause */
    buf[off++] = IEI_CAUSE;  buf[off++] = 1;  buf[off++] = in->cause;

    if (in->has_name) {
        size_t n = strnlen(in->name, sizeof(in->name));
        ROOM(2 + n);
        buf[off++] = IEI_UE_NAME;  buf[off++] = (uint8_t)n;
        memcpy(buf + off, in->name, n);  off += n;
    }
    if (in->has_ts) {
        ROOM(2 + 8);
        buf[off++] = IEI_TIMESTAMP;  buf[off++] = 8;
        wr_u32be(buf + off,     (uint32_t)(in->ts_ms >> 32));
        wr_u32be(buf + off + 4, (uint32_t)(in->ts_ms));
        off += 8;
    }
    for (i = 0; i < in->unknown_num; i++) {        /* 모르는 IE 는 원문 그대로 */
        const unknown_ie_t *u = &in->unknown[i];
        ROOM(2 + (size_t)u->len);                  /* len <= 128 이라 L 은 1바이트 */
        buf[off++] = u->iei;  buf[off++] = (uint8_t)u->len;
        if (u->len > 0) memcpy(buf + off, u->val, u->len);
        off += u->len;
    }

    *out_len = off;
    return RET_OK;
}
