# HINT2 (막혔을 때만)
```
/* tlv.h */
/* [ ] 1. IEI 상수, NAME_LEN_MAX 32, UNKNOWN_NUM_MAX 8, UNKNOWN_VAL_MAX 128 */
/* [ ] 2. unknown_ie_t / msg_body_t (§1-A 표) */
/* [ ] 3. 선언: tlv_parse / tlv_build */

/* tlv.c */
/* [ ] 1. NEED(n) 매크로 — D4 와 모양은 같지만 모자라면 RET_BAD_FORMAT
 *        (입력이 모자란 것은 '규격 위반' 이다.  D4 의 NEED 는 TOO_SMALL) */

/* [ ] 2. keep_unknown(out, t, v, l)
 *        unknown_num >= 8 이거나 l > 128 이면 WRN 로그 후 return (버리되 죽지 않는다)
 *        아니면 슬롯에 iei / len / val 을 복사 (l == 0 이면 memcpy 안 함) */

/* [ ] 3. static int parse_ies(buf, len, out, depth)       ← Nested 를 위해 depth 를 받는다
 *        while (off < len)
 *            NEED(2);  t = buf[off++];  l = buf[off++];             ← 검사 ①
 *            l == 0xFF 이면 NEED(2) 후 rd_u16be 로 확장 길이
 *            NEED(l);                                              ← 검사 ② (제일 중요)
 *            switch (t)
 *                SessionId : l != 4 → BAD_FORMAT / 처음이면 저장, 아니면 WRN (첫 번째 채택)
 *                Cause     : l != 1 → BAD_FORMAT / > 63 → BAD_FORMAT
 *                UeName    : l > 32 → BAD_FORMAT / memcpy 후 name[l] = '\0'
 *                Timestamp : l != 8 → BAD_FORMAT / 상위 4 + 하위 4 바이트
 *                Nested    : depth >= 1 이면 BAD_FORMAT, 아니면 parse_ies(..., depth + 1)
 *                default   : keep_unknown   (에러 아님)
 *            off += l;                                             ← 전진은 루프 끝 한 곳 */

/* [ ] 4. int tlv_parse(buf, len, out)
 *        가드 → memset(out) → parse_ies(.., 0)
 *        순회가 끝난 뒤 필수 IE 검사 (SessionId, Cause 없으면 BAD_FORMAT) */

/* [ ] 5. int tlv_build(in, buf, buf_len, out_len)
 *        가드 / *out_len = 0
 *        ROOM(n) 매크로: 모자라면 RET_TOO_SMALL
 *        SessionId → Cause → UeName(has_name) → Timestamp(has_ts) → unknown[] 원문 순으로 쓴다
 *        길이가 254 이하면 L 1바이트 (A 의 build 는 확장 길이를 쓰지 않는다)
 *        *out_len = off, return RET_OK */
```
