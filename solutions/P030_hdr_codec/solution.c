#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* P030 hdr_codec — 모범답안 */
#define HDR_SIZE 8

typedef struct {
	uint8_t ver;
	uint8_t type;
	uint16_t len;
	uint32_t id;
} hdr_t;

static int hdr_validate(uint8_t ver, uint8_t type, uint16_t len)
{
	if (ver != 1 || type > 3)
		return -2;
	if (len > 1024)
		return -3;
	return 0;
}

int hdr_encode(const hdr_t *h, uint8_t *buf, size_t size)
{
	int rc;

	if (h == NULL || buf == NULL || size < HDR_SIZE)
		return -1;
	rc = hdr_validate(h->ver, h->type, h->len);    /* 검증을 통과한 것만 내보낸다 */
	if (rc != 0)
		return rc;

	buf[0] = h->ver;                               /* 구조체 memcpy 는 패딩·엔디안이 새어 나온다 */
	buf[1] = h->type;
	buf[2] = (uint8_t)(h->len >> 8);
	buf[3] = (uint8_t)(h->len & 0xFF);
	buf[4] = (uint8_t)(h->id >> 24);
	buf[5] = (uint8_t)((h->id >> 16) & 0xFF);
	buf[6] = (uint8_t)((h->id >> 8) & 0xFF);
	buf[7] = (uint8_t)(h->id & 0xFF);
	return HDR_SIZE;
}

int hdr_decode(const uint8_t *buf, size_t size, hdr_t *h)
{
	uint8_t ver, type;
	uint16_t len;
	uint32_t id;
	int rc;

	if (buf == NULL || h == NULL || size < HDR_SIZE)
		return -1;
	ver = buf[0];
	type = buf[1];
	len = (uint16_t)(((uint16_t)buf[2] << 8) | buf[3]);
	id = ((uint32_t)buf[4] << 24) | ((uint32_t)buf[5] << 16) | ((uint32_t)buf[6] << 8) | buf[7];
	rc = hdr_validate(ver, type, len);             /* 검증 후에만 출력 */
	if (rc != 0)
		return rc;
	h->ver = ver;
	h->type = type;
	h->len = len;
	h->id = id;
	return 0;
}
