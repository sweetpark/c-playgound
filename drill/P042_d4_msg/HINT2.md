# HINT2 (막혔을 때만)
```
/* be.h — 엔디안은 바이트 단위로 직접 (htons 금지) */
/* [ ] 1. 가드 BE_H, <stdint.h> */
/* [ ] 2. rd_u16be / rd_u24be / rd_u32be : static inline, 각 바이트를 (uint32_t) 로 올려서 시프트 */
/* [ ] 3. wr_u16be / wr_u24be / wr_u32be : v >> 8k 를 (uint8_t) 로 잘라 b[0] 부터 */
/* [ ] 4. (C 에서) rd_u64be / wr_u64be */

/* msg.h */
/* [ ] 1. 상수: HDR_LEN 12 / CSUM_LEN 2 / PAYLOAD_MAX 1400 */
/* [ ] 2. msg_t (§1-A 표) */
/* [ ] 3. 선언: inet_csum, msg_pack, msg_unpack, hexdump */

/* msg.c */
/* [ ] 1. inet_csum(b, len)
 *        2바이트씩 BE 로 더한다 (uint32_t 누적)
 *        홀수 바이트가 남으면 (uint32_t)b[i] << 8 을 더한다
 *        while (sum >> 16) 으로 접는다
 *        ~sum & 0xFFFF 를 돌려준다 */

/* [ ] 2. NEED(n) 매크로
 *        (size_t)n > buf_len - off 이면 로그 + return RET_TOO_SMALL
 *        (off <= buf_len 이 불변식이라 뺄셈이 안전하다) */

/* [ ] 3. msg_pack(in, buf, buf_len, out_len)
 *        가드: in / buf / out_len
 *        범위: ver > 7, type > 31, prio > 15 → INVALID_ARG
 *              payload_len > 1400, seq > 0xFFFFFF → BAD_FORMAT
 *        *out_len = 0
 *        NEED(HDR_LEN + payload_len + CSUM_LEN)     ← 한 번에 전체 공간 확보
 *        옥텟 0   : ((ver & 7) << 5) | (type & 0x1F)
 *        옥텟 1   : flags (모르는 비트도 그대로)
 *        옥텟 2-3 : wr_u16be(payload_len)
 *        옥텟 4-7 : wr_u32be(session_id)
 *        옥텟 8-10: wr_u24be(seq)
 *        옥텟 11  : (prio & 0x0F) << 4              ← Reserved 는 0 강제
 *        payload 를 복사
 *        체크섬: wr_u16be(buf + off, inet_csum(buf, off))
 *        *out_len = off, return RET_OK */

/* [ ] 4. msg_unpack(buf, buf_len, out, consumed)
 *        가드 / out 전부 0 / *consumed = 0
 *        NEED(HDR_LEN)                                ← 헤더부터
 *        ver, type, flags, plen, session_id, seq, prio 를 꺼낸다 (시프트 후 마스킹)
 *        Reserved (옥텟 11 의 하위 4비트) 가 0 이 아니면 BAD_FORMAT
 *        plen > 1400 이면 BAD_FORMAT                  ← 길이 필드 자체를 의심
 *        NEED(plen + CSUM_LEN)                        ← 실제로 그만큼 있나
 *        payload 복사
 *        체크섬 비교 (rx != calc 이면 BAD_FORMAT)
 *        *consumed = off, return RET_OK */
```
