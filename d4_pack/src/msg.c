#include "common.h"
#include "log.h"
#include "guard.h"
#include "msg.h"

/* ── 인터넷 체크섬 (1의 보수 합) — static 이 아니다. msg.h 에 선언하고 시험이 직접 부른다 ── */
uint16_t inet_csum(const uint8_t *b, size_t len)
{
    uint32_t sum = 0;
    size_t   i   = 0;

    for (i = 0; i + 1 < len; i += 2) sum += rd_u16be(b + i);
    if (i < len) sum += (uint32_t)b[i] << 8;          /* 홀수 바이트 패딩 */
    while (sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);
    return (uint16_t)(~sum & 0xFFFF);
}

/* ── 공간 검사: 쓰기/읽기 앞에 무조건 ─────────────────────── */
#define NEED(_n)                                                        \
    do {                                                                \
        if ((size_t)(_n) > buf_len - off) {                             \
            LOG_ERR("need %zu but %zu left (%s/%d)",                    \
                    (size_t)(_n), buf_len - off, __func__, __LINE__);   \
            return RET_TOO_SMALL;                                       \
        }                                                               \
    } while (0)

int msg_pack(const msg_t *in, uint8_t *buf, size_t buf_len, size_t *out_len)
{
    size_t off = 0;

    CHK_PTR(in);
    CHK_PTR(buf);
    CHK_PTR(out_len);
    *out_len = 0;                                 /* ★ 실패하면 0 */

    /* uint8_t 에 '< 0' 비교를 넣지 않는다 (-Wtype-limits 가 -Werror 에서 깨진다) */
    if (in->ver > 7 || in->type > 31 || in->prio > 15) {
        LOG_ERR("ver/type/prio out of range (%s/%d)", __func__, __LINE__);
        return RET_INVALID_ARG;
    }
    if (in->payload_len > PAYLOAD_MAX) return RET_BAD_FORMAT;
    if (in->seq > 0xFFFFFFu)           return RET_BAD_FORMAT;

    NEED(HDR_LEN + in->payload_len + CSUM_LEN);   /* ★ 한 번에 전체 공간 확보 */

    buf[off++] = (uint8_t)(((in->ver & 0x07) << 5) | (in->type & 0x1F));
    buf[off++] = in->flags;                       /* 모르는 비트도 그대로 */
    wr_u16be(buf + off, in->payload_len);  off += 2;
    wr_u32be(buf + off, in->session_id);   off += 4;
    wr_u24be(buf + off, in->seq);          off += 3;
    buf[off++] = (uint8_t)((in->prio & 0x0F) << 4);   /* Reserved 는 0 강제 */

    if (in->payload_len > 0) {
        memcpy(buf + off, in->payload, in->payload_len);
        off += in->payload_len;
    }

    wr_u16be(buf + off, inet_csum(buf, off));   off += CSUM_LEN;

    *out_len = off;
    return RET_OK;
}

int msg_unpack(const uint8_t *buf, size_t buf_len, msg_t *out, size_t *consumed)
{
    size_t   off  = 0;
    uint16_t plen = 0, csum_rx = 0, csum_calc = 0;
    uint8_t  b11  = 0;

    CHK_PTR(buf);
    CHK_PTR(out);
    CHK_PTR(consumed);

    memset(out, 0x00, sizeof(*out));
    *consumed = 0;

    NEED(HDR_LEN);                                   /* ① 헤더부터 */

    out->ver   = (uint8_t)((buf[0] >> 5) & 0x07);
    out->type  = (uint8_t)( buf[0]       & 0x1F);
    out->flags = buf[1];
    plen       = rd_u16be(buf + 2);
    out->session_id = rd_u32be(buf + 4);
    out->seq        = rd_u24be(buf + 8);

    b11 = buf[11];
    out->prio = (uint8_t)((b11 >> 4) & 0x0F);
    if ((b11 & 0x0F) != 0) {                         /* ② Reserved 검사 */
        LOG_ERR("reserved=0x%X", b11 & 0x0F);
        return RET_BAD_FORMAT;
    }
    off = HDR_LEN;

    if (plen > PAYLOAD_MAX) {                        /* ③ 길이 필드 자체를 의심 */
        LOG_ERR("payload_len=%u > max", plen);
        return RET_BAD_FORMAT;
    }
    NEED((size_t)plen + CSUM_LEN);                   /* ④ 실제로 그만큼 있나 */

    if (plen > 0) memcpy(out->payload, buf + off, plen);
    out->payload_len = plen;
    off += plen;

    csum_rx   = rd_u16be(buf + off);
    csum_calc = inet_csum(buf, off);
    if (csum_rx != csum_calc) {                      /* ⑤ 무결성 */
        LOG_ERR("csum rx=0x%04X calc=0x%04X", csum_rx, csum_calc);
        return RET_BAD_FORMAT;
    }
    off += CSUM_LEN;

    *consumed = off;                                 /* ⑥ 몇 바이트 먹었는지 */
    return RET_OK;
}
