/* P043 tlv — 요구사항은 problem.md. 헤더(*.h)의 선언을 보고 함수를 채운다. */
#include "common.h"
#include "log.h"
#include "guard.h"
#include "be.h"
#include "tlv.h"

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

int tlv_parse(const uint8_t *buf, size_t buf_len, msg_body_t *out)
{
	(void)buf;
	(void)buf_len;
	(void)out;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

#define ROOM(_n)                                                        \
    do {                                                                \
        if ((size_t)(_n) > buf_len - off) return RET_TOO_SMALL;         \
    } while (0)

int tlv_build(const msg_body_t *in, uint8_t *buf, size_t buf_len, size_t *out_len)
{
	(void)in;
	(void)buf;
	(void)buf_len;
	(void)out_len;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

