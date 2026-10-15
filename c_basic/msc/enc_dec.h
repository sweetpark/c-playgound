#ifndef _ENC_DEC_H_
#define _ENC_DEC_H_

#include <stdint.h>
#include <stddef.h>

/*
 * 메시지 헤더 encode / decode
 *
 * [와이어(네트워크) 포맷: 총 16바이트, 모든 다바이트 필드는 network byte order(빅엔디안)]
 *
 *   offset  size  field
 *   ------  ----  ----------
 *      0      1   version        (1 만 유효)
 *      1      1   type           (1:REQUEST 2:RESPONSE 3:NOTIFY)
 *      2      2   length         (body 의 길이)
 *      4      4   seq
 *      8      8   session_id
 *
 * 구조체(msg_hdr_t)의 메모리 배치와 와이어 포맷은 별개이다.
 * 구조체를 통째로 memcpy 하면 호스트 엔디안과 패딩(컴파일러/플랫폼마다 다름)이 그대로 노출되므로,
 * 반드시 필드 단위로 변환해서 읽고 쓴다.
 */

#define MSG_HDR_SIZE		16		/* 와이어 포맷상 헤더 크기 */
#define MSG_HDR_VERSION		1		/* 유효한 version 값 */

/* msg_hdr_t.type 에 들어갈 수 있는 값 */
typedef enum _msg_type_e_
{
	MSG_TYPE_REQUEST  = 1,
	MSG_TYPE_RESPONSE = 2,
	MSG_TYPE_NOTIFY   = 3
} msg_type_e;

/* 반환값: 성공은 0, 실패는 원인별 음수 */
typedef enum _hdr_rc_e_
{
	HDR_RC_SUCC              =  0,
	HDR_RC_ERR_INVALID_PARAM = -1,	/* NULL 포인터 등 잘못된 인자 */
	HDR_RC_ERR_BUF_SMALL     = -2,	/* 버퍼 길이가 MSG_HDR_SIZE(16) 미만 */
	HDR_RC_ERR_VERSION       = -3,	/* version 이 1 이 아님 */
	HDR_RC_ERR_TYPE          = -4	/* 정의되지 않은 type */
} hdr_rc_e;

typedef struct{
	uint8_t version;		/* 1만 유효 */
	uint8_t type;			/* 1:REQUEST 2:RESPONSE 3:NOTIFY */
	uint16_t length;		/* body의 길이 */
	uint32_t seq;
	uint64_t session_id;
} msg_hdr_t;

/*
 * hdr_encode() - hdr 를 network byte order 의 16바이트로 buf 에 저장
 *   buf_size 는 buf 의 크기이며 16 이상이어야 한다 (16바이트만 쓰고, 그 이후는 건드리지 않음).
 *   실패하면 buf 는 변경하지 않는다.
 *   반환: 0 성공 / HDR_RC_ERR_INVALID_PARAM / HDR_RC_ERR_BUF_SMALL / HDR_RC_ERR_VERSION / HDR_RC_ERR_TYPE
 */
int hdr_encode(const msg_hdr_t *hdr, uint8_t *buf, size_t buf_size);

/*
 * hdr_decode() - buf 의 16바이트를 host byte order 의 구조체로 변환해서 hdr 에 저장
 *   buf_len 은 buf 에 들어 있는 유효 데이터 길이이며 16 이상이어야 한다
 *   (16 보다 크면 body 가 뒤에 붙은 것으로 보고 앞 16바이트만 해석).
 *   실패하면 hdr 는 변경하지 않는다.
 *   반환값은 hdr_encode 와 같다.
 */
int hdr_decode(const uint8_t *buf, size_t buf_len, msg_hdr_t *hdr);

/* type 의 이름 문자열 반환, 정의되지 않은 값이면 "UNKNOWN" */
const char *msg_type_str(uint8_t type);

#endif /* _ENC_DEC_H_ */
