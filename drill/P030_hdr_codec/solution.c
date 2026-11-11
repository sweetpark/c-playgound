#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* P030 hdr_codec — 요구사항은 problem.md */
#define HDR_SIZE 8

typedef struct {
	uint8_t ver;      /* 1 만 유효 */
	uint8_t type;     /* 0..3 */
	uint16_t len;     /* 0..1024 */
	uint32_t id;
} hdr_t;

int hdr_encode(const hdr_t *h, uint8_t *buf, size_t size)
{
	(void)h;
	(void)buf;
	(void)size;
	/* TODO */
	return -99;
}

int hdr_decode(const uint8_t *buf, size_t size, hdr_t *h)
{
	(void)buf;
	(void)size;
	(void)h;
	/* TODO */
	return -99;
}
