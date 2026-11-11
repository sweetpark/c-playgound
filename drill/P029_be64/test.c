#include <stdint.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

#define PAD 0x7E

static void enc_case(uint64_t v, const uint8_t want[8])
{
	uint8_t buf[16];
	memset(buf, PAD, sizeof(buf));
	put_be64(buf + 4, v);
	CHECK(memcmp(buf + 4, want, 8) == 0);
	CHECK(buf[3] == PAD && buf[12] == PAD && buf[0] == PAD && buf[15] == PAD);   /* 8바이트만 쓴다 */
	CHECK(get_be64(buf + 4) == v);                                              /* 왕복 */
}

int main(void)
{
	static const uint8_t a[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
	static const uint8_t z[8] = {0};
	static const uint8_t f[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	static const uint8_t h[8] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
	static const uint8_t t[8] = {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	static const uint8_t l[8] = {0x00, 0x00, 0x00, 0x00, 0xDE, 0xAD, 0xBE, 0xEF};
	uint64_t x = 88172645463325252ULL;
	int i;
	int ok = 1;

	enc_case(0x0102030405060708ULL, a);
	enc_case(0, z);
	enc_case(UINT64_MAX, f);
	enc_case(0x8000000000000001ULL, h);
	enc_case(0xFF00000000000000ULL, t);   /* 최상위 바이트: int 로 시프트하면 부호/폭 문제 (UBSan) */
	enc_case(0xDEADBEEFULL, l);

	/* 디코드: 바이트 배열 -> 값 */
	CHECK(get_be64(a) == 0x0102030405060708ULL);
	CHECK(get_be64(f) == UINT64_MAX);
	CHECK(get_be64(t) == 0xFF00000000000000ULL);

	/* 의사 난수 왕복 */
	for (i = 0; i < 2000; i++) {
		uint8_t b[8];
		uint64_t back;
		x ^= x << 13; x ^= x >> 7; x ^= x << 17;
		put_be64(b, x);
		back = get_be64(b);
		if (back != x) ok = 0;
		if (b[0] != (uint8_t)(x >> 56) || b[7] != (uint8_t)x) ok = 0;
	}
	CHECK(ok);
	return drill_report();
}
