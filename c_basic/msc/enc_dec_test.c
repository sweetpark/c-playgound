/*
 * enc_dec 테스트 프로그램
 *
 * 빌드: gcc -Wall -Wextra -o enc_dec_test enc_dec_test.c enc_dec.c
 * 실행: ./enc_dec_test      (전부 통과하면 종료코드 0, 하나라도 실패하면 1)
 *
 * 1. 헤더를 encode 한 16바이트를 hex값으로 출력하여 byte 순서 확인 (01020304 > 01 02 03 04 로 출력)
 * 2. encode 한 결과를 다시 decode하여 모든 필드가 원본과 동일한지 확인
 * 3. 잘못된 version, 정의되지 않은 type, 길이가 부적한 버퍼에 대해 에러 리턴되는지 확인
 * (+ msg_type_str, NULL 인자, 실패 시 출력 버퍼/구조체가 변경되지 않는지도 확인)
 */

#include <stdio.h>
#include <string.h>
#include "enc_dec.h"

static int g_total = 0;
static int g_fail  = 0;

/* 조건이 거짓이면 FAIL 출력. 어느 검사인지 알 수 있게 설명 문자열을 같이 받는다 */
#define CHECK(cond, name) \
	do { \
		g_total++; \
		if (cond) { \
			printf("  [PASS] %s\n", name); \
		} else { \
			g_fail++; \
			printf("  [FAIL] %s  (%s:%d)\n", name, __FILE__, __LINE__); \
		} \
	} while (0)

static void print_hex(const char *label, const uint8_t *p, size_t n)
{
	size_t i;

	printf("  %s:", label);
	for (i = 0; i < n; i++) {
		printf(" %02X", p[i]);
	}
	printf("\n");
}

/* 모든 필드가 같으면 1 */
static int hdr_equal(const msg_hdr_t *a, const msg_hdr_t *b)
{
	return a->version == b->version && a->type == b->type && a->length == b->length &&
	       a->seq == b->seq && a->session_id == b->session_id;
}

/* 1. byte 순서 확인: 필드를 01 02 03 ... 순서로 채우면 hex 출력이 01 02 03 ... 으로 나와야 한다 */
static void test_byte_order(void)
{
	/* 값이 바이트마다 달라서 순서/위치가 틀리면 바로 드러난다 */
	msg_hdr_t hdr = { 1, 2, 0x0304, 0x05060708, 0x090A0B0C0D0E0F10ULL };
	const uint8_t expect[MSG_HDR_SIZE] = {
		0x01,						/* version */
		0x02,						/* type */
		0x03, 0x04,					/* length */
		0x05, 0x06, 0x07, 0x08,		/* seq */
		0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10		/* session_id */
	};
	uint8_t buf[MSG_HDR_SIZE];

	printf("[1] encode 결과의 byte 순서 확인\n");
	memset(buf, 0xAA, sizeof(buf));

	CHECK(hdr_encode(&hdr, buf, sizeof(buf)) == 0, "encode 성공(0)");
	print_hex("encoded", buf, sizeof(buf));
	print_hex("expect ", expect, sizeof(expect));
	CHECK(memcmp(buf, expect, MSG_HDR_SIZE) == 0, "network byte order(빅엔디안) 와 일치");
}

/* 2. encode -> decode 왕복 */
static void test_roundtrip(void)
{
	msg_hdr_t cases[] = {
		{ 1, MSG_TYPE_REQUEST,  0x0000, 0x00000000, 0x0000000000000000ULL },	/* 전부 0 */
		{ 1, MSG_TYPE_RESPONSE, 0x1234, 0x12345678, 0x123456789ABCDEF0ULL },
		{ 1, MSG_TYPE_NOTIFY,   0xFFFF, 0xFFFFFFFF, 0xFFFFFFFFFFFFFFFFULL },	/* 전부 최대값(부호 확장 확인) */
		{ 1, MSG_TYPE_REQUEST,  0x8000, 0x80000000, 0x8000000000000001ULL }		/* 최상위 비트 켜짐 */
	};
	int n = (int)(sizeof(cases) / sizeof(cases[0]));
	int i;

	printf("[2] encode -> decode 왕복\n");
	for (i = 0; i < n; i++) {
		uint8_t buf[MSG_HDR_SIZE];
		msg_hdr_t out;
		char name[64];

		memset(&out, 0, sizeof(out));
		snprintf(name, sizeof(name), "case %d: 모든 필드 동일 (type=%s)", i, msg_type_str(cases[i].type));

		CHECK(hdr_encode(&cases[i], buf, sizeof(buf)) == 0 && hdr_decode(buf, sizeof(buf), &out) == 0 &&
		      hdr_equal(&cases[i], &out), name);
	}

	/* 버퍼가 16보다 크면(body 가 붙어 있는 경우) 앞 16바이트만 쓰고/읽어야 한다 */
	{
		msg_hdr_t hdr = { 1, MSG_TYPE_NOTIFY, 7, 9, 11 };
		msg_hdr_t out;
		uint8_t big[32];
		int k, untouched = 1;

		memset(big, 0xAA, sizeof(big));
		CHECK(hdr_encode(&hdr, big, sizeof(big)) == 0, "큰 버퍼(32)에 encode 성공");
		for (k = MSG_HDR_SIZE; k < (int)sizeof(big); k++) {
			if (big[k] != 0xAA) {
				untouched = 0;
			}
		}
		CHECK(untouched, "16바이트 이후는 건드리지 않음");
		CHECK(hdr_decode(big, sizeof(big), &out) == 0 && hdr_equal(&hdr, &out), "큰 버퍼(32)에서 decode 성공");
	}
}

/* 3. 에러 리턴 확인 */
static void test_errors(void)
{
	msg_hdr_t good = { 1, MSG_TYPE_REQUEST, 10, 20, 30 };
	msg_hdr_t hdr, out, out_before;
	uint8_t buf[MSG_HDR_SIZE], before[MSG_HDR_SIZE], good_buf[MSG_HDR_SIZE];
	int i;

	printf("[3] 에러 리턴 확인\n");

	/* --- 잘못된 version --- */
	hdr = good;
	hdr.version = 0;
	memset(buf, 0xAA, sizeof(buf));
	memcpy(before, buf, sizeof(buf));
	CHECK(hdr_encode(&hdr, buf, sizeof(buf)) == HDR_RC_ERR_VERSION, "encode: version=0 -> ERR_VERSION");
	CHECK(memcmp(buf, before, sizeof(buf)) == 0, "encode 실패 시 buf 불변");
	hdr.version = 2;
	CHECK(hdr_encode(&hdr, buf, sizeof(buf)) == HDR_RC_ERR_VERSION, "encode: version=2 -> ERR_VERSION");

	/* --- 정의되지 않은 type (0, 4, 255) --- */
	{
		const uint8_t bad_types[] = { 0, 4, 255 };
		for (i = 0; i < (int)sizeof(bad_types); i++) {
			char name[64];

			hdr = good;
			hdr.type = bad_types[i];
			snprintf(name, sizeof(name), "encode: type=%u -> ERR_TYPE", bad_types[i]);
			CHECK(hdr_encode(&hdr, buf, sizeof(buf)) == HDR_RC_ERR_TYPE, name);
		}
	}

	/* --- 버퍼 길이 부족 (encode: 15, 0 / decode: 15, 0) --- */
	CHECK(hdr_encode(&good, buf, MSG_HDR_SIZE - 1) == HDR_RC_ERR_BUF_SMALL, "encode: buf_size=15 -> ERR_BUF_SMALL");
	CHECK(hdr_encode(&good, buf, 0) == HDR_RC_ERR_BUF_SMALL, "encode: buf_size=0 -> ERR_BUF_SMALL");

	/* decode 쪽 에러는 정상 encode 결과를 일부러 망가뜨려서 만든다 */
	hdr_encode(&good, good_buf, sizeof(good_buf));

	memset(&out, 0x5A, sizeof(out));
	memcpy(&out_before, &out, sizeof(out));
	CHECK(hdr_decode(good_buf, MSG_HDR_SIZE - 1, &out) == HDR_RC_ERR_BUF_SMALL, "decode: buf_len=15 -> ERR_BUF_SMALL");
	CHECK(hdr_decode(good_buf, 0, &out) == HDR_RC_ERR_BUF_SMALL, "decode: buf_len=0 -> ERR_BUF_SMALL");

	memcpy(buf, good_buf, sizeof(buf));
	buf[0] = 2;
	CHECK(hdr_decode(buf, sizeof(buf), &out) == HDR_RC_ERR_VERSION, "decode: version=2 -> ERR_VERSION");
	buf[0] = 0;
	CHECK(hdr_decode(buf, sizeof(buf), &out) == HDR_RC_ERR_VERSION, "decode: version=0 -> ERR_VERSION");

	memcpy(buf, good_buf, sizeof(buf));
	buf[1] = 4;
	CHECK(hdr_decode(buf, sizeof(buf), &out) == HDR_RC_ERR_TYPE, "decode: type=4 -> ERR_TYPE");
	buf[1] = 0;
	CHECK(hdr_decode(buf, sizeof(buf), &out) == HDR_RC_ERR_TYPE, "decode: type=0 -> ERR_TYPE");

	CHECK(memcmp(&out, &out_before, sizeof(out)) == 0, "decode 실패 시 hdr 불변");

	/* --- NULL 인자 --- */
	CHECK(hdr_encode(NULL, buf, sizeof(buf)) == HDR_RC_ERR_INVALID_PARAM, "encode: hdr=NULL -> ERR_INVALID_PARAM");
	CHECK(hdr_encode(&good, NULL, sizeof(buf)) == HDR_RC_ERR_INVALID_PARAM, "encode: buf=NULL -> ERR_INVALID_PARAM");
	CHECK(hdr_decode(NULL, sizeof(buf), &out) == HDR_RC_ERR_INVALID_PARAM, "decode: buf=NULL -> ERR_INVALID_PARAM");
	CHECK(hdr_decode(good_buf, sizeof(good_buf), NULL) == HDR_RC_ERR_INVALID_PARAM, "decode: hdr=NULL -> ERR_INVALID_PARAM");
}

/* msg_type_str */
static void test_type_str(void)
{
	printf("[4] msg_type_str\n");
	CHECK(strcmp(msg_type_str(1), "REQUEST") == 0,  "1 -> REQUEST");
	CHECK(strcmp(msg_type_str(2), "RESPONSE") == 0, "2 -> RESPONSE");
	CHECK(strcmp(msg_type_str(3), "NOTIFY") == 0,   "3 -> NOTIFY");
	CHECK(strcmp(msg_type_str(0), "UNKNOWN") == 0,  "0 -> UNKNOWN");
	CHECK(strcmp(msg_type_str(4), "UNKNOWN") == 0,  "4 -> UNKNOWN");
	CHECK(strcmp(msg_type_str(255), "UNKNOWN") == 0, "255 -> UNKNOWN");
}

int main(void)
{
	test_byte_order();
	test_roundtrip();
	test_errors();
	test_type_str();

	printf("\n%s: %d/%d passed\n", g_fail ? "FAILED" : "ALL PASSED", g_total - g_fail, g_total);

	return g_fail ? 1 : 0;
}
