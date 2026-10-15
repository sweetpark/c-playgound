#include <stdio.h>
#include "enc_dec.h"

/*
 *hdr_encode() - 헤더를 network byte order의 16바이트로 buf에 저장. 구조체를 통째로 memcpy() 하지 말고 필드단위 변환 저장
 *hdr_decode() - buf의 16바이트를 host byte order의 구조체로 반환
 두함수 모두 성공시 0, 실패시 원인별로 음수값 리턴
 msg_type_str() - switch문을 사용하여 type의 이름 문자열을 반환 정의 되지않으면 UNKNOWN
 * */

/*
 * [바이트 단위 빅엔디안 읽기/쓰기]
 *  htons/htonl 로 바꾼 값을 memcpy 하는 대신, 값을 시프트로 한 바이트씩 직접 넣고 뺀다.
 *   - 호스트 엔디안과 무관하게 동작한다 (시프트는 "값" 기준이라 메모리 배치와 상관없음).
 *   - buf + offset 이 정렬되어 있지 않아도 된다 (uint32_t* 로 캐스팅해서 쓰면 정렬 문제가 생길 수 있음).
 *  최상위 바이트가 가장 낮은 주소로 간다.  예) 0x01020304 -> p[0]=01 p[1]=02 p[2]=03 p[3]=04
 */
static void put_be16(uint8_t *p, uint16_t v)
{
	p[0] = (uint8_t)(v >> 8);
	p[1] = (uint8_t)(v);
}

static void put_be32(uint8_t *p, uint32_t v)
{
	p[0] = (uint8_t)(v >> 24);
	p[1] = (uint8_t)(v >> 16);
	p[2] = (uint8_t)(v >> 8);
	p[3] = (uint8_t)(v);
}

static void put_be64(uint8_t *p, uint64_t v)
{
	put_be32(p,     (uint32_t)(v >> 32));
	put_be32(p + 4, (uint32_t)(v));
}

static uint16_t get_be16(const uint8_t *p)
{
	return (uint16_t)(((uint16_t)p[0] << 8) | p[1]);
}

static uint32_t get_be32(const uint8_t *p)
{
	/* 시프트 전에 uint32_t 로 승격해야 한다 (uint8_t 는 int 로 승격되어 << 24 에서 부호 비트가 문제될 수 있음) */
	return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

static uint64_t get_be64(const uint8_t *p)
{
	return ((uint64_t)get_be32(p) << 32) | get_be32(p + 4);
}

/* version 과 type 이 유효한지 검사. 유효하면 0, 아니면 원인별 음수 */
static int hdr_validate(uint8_t version, uint8_t type)
{
	if (version != MSG_HDR_VERSION) {
		return HDR_RC_ERR_VERSION;
	}

	switch (type) {
	case MSG_TYPE_REQUEST:
	case MSG_TYPE_RESPONSE:
	case MSG_TYPE_NOTIFY:
		return HDR_RC_SUCC;
	default:
		return HDR_RC_ERR_TYPE;
	}
}

int hdr_encode(const msg_hdr_t *hdr, uint8_t *buf, size_t buf_size)
{
	int rc;

	/* 1. 인자 검사 -> 2. 버퍼 크기 -> 3. 값 유효성. 모두 통과한 뒤에만 buf 에 쓴다 (실패 시 buf 불변) */
	if (hdr == NULL || buf == NULL) {
		return HDR_RC_ERR_INVALID_PARAM;
	}

	if (buf_size < MSG_HDR_SIZE) {
		return HDR_RC_ERR_BUF_SMALL;
	}

	if ((rc = hdr_validate(hdr->version, hdr->type)) != HDR_RC_SUCC) {
		return rc;
	}

	/* 필드 단위로 와이어 포맷의 offset 에 맞춰 저장 (구조체 통째 memcpy 금지) */
	buf[0] = hdr->version;				/* offset 0, 1바이트 : 엔디안 무관 */
	buf[1] = hdr->type;					/* offset 1, 1바이트 */
	put_be16(buf + 2, hdr->length);		/* offset 2, 2바이트 */
	put_be32(buf + 4, hdr->seq);		/* offset 4, 4바이트 */
	put_be64(buf + 8, hdr->session_id);	/* offset 8, 8바이트 */

	return HDR_RC_SUCC;
}

int hdr_decode(const uint8_t *buf, size_t buf_len, msg_hdr_t *hdr)
{
	msg_hdr_t tmp;		/* 검증이 끝나기 전에는 호출자의 hdr 를 건드리지 않기 위한 임시 변수 */
	int rc;

	if (buf == NULL || hdr == NULL) {
		return HDR_RC_ERR_INVALID_PARAM;
	}

	if (buf_len < MSG_HDR_SIZE) {
		return HDR_RC_ERR_BUF_SMALL;
	}

	tmp.version    = buf[0];
	tmp.type       = buf[1];
	tmp.length     = get_be16(buf + 2);
	tmp.seq        = get_be32(buf + 4);
	tmp.session_id = get_be64(buf + 8);

	/* 외부(네트워크)에서 온 데이터이므로 반드시 검증한다 */
	if ((rc = hdr_validate(tmp.version, tmp.type)) != HDR_RC_SUCC) {
		return rc;
	}

	*hdr = tmp;		/* 구조체 대입: 이미 host byte order 의 값들이므로 안전 */

	return HDR_RC_SUCC;
}

const char *msg_type_str(uint8_t type)
{
	switch (type) {
	case MSG_TYPE_REQUEST:
		return "REQUEST";
	case MSG_TYPE_RESPONSE:
		return "RESPONSE";
	case MSG_TYPE_NOTIFY:
		return "NOTIFY";
	default:
		return "UNKNOWN";
	}
}
