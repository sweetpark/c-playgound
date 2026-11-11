#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

#define PAD 0x7E

static int bytes_eq(const uint8_t *p, const uint8_t *w, size_t n)
{
	return memcmp(p, w, n) == 0;
}

int main(void)
{
	static const uint8_t wire[8] = {0x01, 0x02, 0x02, 0x34, 0xA1, 0xB2, 0xC3, 0xD4};
	hdr_t h = {1, 2, 0x0234, 0xA1B2C3D4u};
	hdr_t d;
	uint8_t buf[16];
	uint8_t *heap;

	/* 인코드: 필드 단위, 빅엔디안, 구조체 통째 memcpy 아님 */
	memset(buf, PAD, sizeof(buf));
	CHECK(hdr_encode(&h, buf, 8) == 8);
	CHECK(bytes_eq(buf, wire, 8));
	CHECK(buf[8] == PAD);                       /* 8바이트 밖은 건드리지 않는다 */

	/* 디코드 */
	memset(&d, 0xAA, sizeof(d));
	CHECK(hdr_decode(wire, 8, &d) == 0);
	CHECK(d.ver == 1 && d.type == 2 && d.len == 0x0234 && d.id == 0xA1B2C3D4u);

	/* 정확히 8바이트 힙 버퍼: 경계 */
	heap = malloc(8);
	if (heap) {
		CHECK(hdr_encode(&h, heap, 8) == 8);
		CHECK(bytes_eq(heap, wire, 8));
		memset(&d, 0, sizeof(d));
		CHECK(hdr_decode(heap, 8, &d) == 0);
		CHECK(d.id == 0xA1B2C3D4u);
		free(heap);
	}

	/* 크기 부족 */
	memset(buf, PAD, sizeof(buf));
	CHECK(hdr_encode(&h, buf, 7) == -1);
	CHECK(buf[0] == PAD);
	memset(&d, 0xAA, sizeof(d));
	CHECK(hdr_decode(wire, 7, &d) == -1);
	CHECK(d.ver == 0xAA);                       /* 실패하면 출력 불변 */

	/* NULL */
	CHECK(hdr_encode(NULL, buf, 8) == -1);
	CHECK(hdr_encode(&h, NULL, 8) == -1);
	CHECK(hdr_decode(NULL, 8, &d) == -1);
	CHECK(hdr_decode(wire, 8, NULL) == -1);

	/* 검증: 버전 / 타입(0..3) / 길이(<=1024) — 검증을 통과한 것만 내보낸다 */
	{
		hdr_t bad;
		uint8_t w[8];
		memcpy(w, wire, 8);

		bad = h; bad.ver = 2;
		memset(buf, PAD, sizeof(buf));
		CHECK(hdr_encode(&bad, buf, 8) == -2);
		CHECK(buf[0] == PAD);
		bad = h; bad.type = 4;
		CHECK(hdr_encode(&bad, buf, 8) == -2);
		bad = h; bad.len = 1025;
		CHECK(hdr_encode(&bad, buf, 8) == -3);
		bad = h; bad.len = 1024; bad.type = 3;
		CHECK(hdr_encode(&bad, buf, 8) == 8);

		w[0] = 9;
		memset(&d, 0xAA, sizeof(d));
		CHECK(hdr_decode(w, 8, &d) == -2);
		CHECK(d.ver == 0xAA);
		memcpy(w, wire, 8); w[1] = 7;
		CHECK(hdr_decode(w, 8, &d) == -2);
		memcpy(w, wire, 8); w[2] = 0x04; w[3] = 0x01;   /* len = 1025 */
		CHECK(hdr_decode(w, 8, &d) == -3);
		memcpy(w, wire, 8); w[2] = 0x04; w[3] = 0x00;   /* len = 1024 */
		CHECK(hdr_decode(w, 8, &d) == 0 && d.len == 1024);
	}

	/* 값 0 / 최대 id */
	{
		hdr_t z = {1, 0, 0, 0xFFFFFFFFu};
		memset(buf, PAD, sizeof(buf));
		CHECK(hdr_encode(&z, buf, 16) == 8);
		CHECK(buf[4] == 0xFF && buf[7] == 0xFF && buf[2] == 0 && buf[3] == 0);
		memset(&d, 0, sizeof(d));
		CHECK(hdr_decode(buf, 16, &d) == 0 && d.id == 0xFFFFFFFFu);
	}

	return drill_report();
}
