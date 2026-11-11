/* P042 msg — 요구사항은 problem.md. 헤더(*.h)의 선언을 보고 함수를 채운다. */
#include "common.h"
#include "log.h"
#include "guard.h"
#include "msg.h"

/* ── 인터넷 체크섬 (1의 보수 합) — static 이 아니다. msg.h 에 선언하고 시험이 직접 부른다 ── */
uint16_t inet_csum(const uint8_t *b, size_t len)
{
	(void)b;
	(void)len;
	/* TODO: problem.md 를 보고 Good 구현 */
	return 0;
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
	(void)in;
	(void)buf;
	(void)buf_len;
	(void)out_len;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

int msg_unpack(const uint8_t *buf, size_t buf_len, msg_t *out, size_t *consumed)
{
	(void)buf;
	(void)buf_len;
	(void)out;
	(void)consumed;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

