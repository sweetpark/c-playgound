#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

/* long long type의 byte ordering 변환 함수 (network to host, host to network)를 작성 */

/*
 * [개념]
 *  - 네트워크 바이트 순서 = 빅엔디안 (높은 자리 바이트가 낮은 주소에 놓임)
 *  - x86 같은 리틀엔디안 호스트는 반대 (낮은 자리 바이트가 낮은 주소에 놓임)
 *  - 0x0102030405060708 이 메모리에 놓이는 모양
 *      빅엔디안   : 01 02 03 04 05 06 07 08   (네트워크)
 *      리틀엔디안 : 08 07 06 05 04 03 02 01   (x86)
 *  - 따라서 리틀엔디안 호스트에서는 8바이트의 순서를 뒤집어야 하고,
 *    빅엔디안 호스트에서는 이미 네트워크 순서이므로 아무것도 하지 않는다.
 *  - host->network, network->host 는 "뒤집기"라는 같은 연산이라 구현이 같다.
 *    (두 번 적용하면 원래 값으로 돌아온다)
 */

/* 호스트가 리틀엔디안이면 1, 빅엔디안이면 0
 * 값 1 을 1바이트 포인터로 읽었을 때 첫 바이트가 1 이면 낮은 바이트가 낮은 주소 = 리틀엔디안 */
static int is_little_endian(void)
{
	uint16_t x = 1;

	return *(unsigned char *)&x == 1;
}

/*
 * 8바이트의 순서를 무조건 뒤집는다.
 *  - 각 바이트를 마스크로 뽑아서(&) 반대 위치로 시프트(<<, >>)한 뒤 OR(|) 로 합친다.
 *  - 바이트 i (0 = 최하위) 는 (7 - i) 번 위치로 가므로 이동량은 (7 - 2i) * 8 비트이다.
 *      byte0 : << 56    byte1 : << 40    byte2 : << 24    byte3 : << 8
 *      byte4 : >> 8     byte5 : >> 24    byte6 : >> 40    byte7 : >> 56
 *  - 연산자 우선순위 주의: 시프트(<<, >>)가 &(AND) 보다 먼저 계산된다.
 *      n & 0xFF << 56  ==  n & (0xFF << 56)  (마스크만 시프트되어 의도와 다름)
 *    이 경우 0xFF 는 int 라서 << 56 이 오버플로(정의되지 않은 동작)이기도 하다.
 *    그래서 (n & 마스크) 를 괄호로 묶은 뒤에 시프트한다. n 이 uint64_t 이므로 결과도 64비트이다.
 *  - 마스크 상수의 ULL 접미사는 여기서는 필수는 아니지만, 64비트 상수라는 것을 드러내려고 붙였다.
 */
static uint64_t swap64(uint64_t n)
{
	return ((n & 0x00000000000000FFULL) << 56) |
	       ((n & 0x000000000000FF00ULL) << 40) |
	       ((n & 0x0000000000FF0000ULL) << 24) |
	       ((n & 0x00000000FF000000ULL) <<  8) |
	       ((n & 0x000000FF00000000ULL) >>  8) |
	       ((n & 0x0000FF0000000000ULL) >> 24) |
	       ((n & 0x00FF000000000000ULL) >> 40) |
	       ((n & 0xFF00000000000000ULL) >> 56);
}

/* host -> network (64bit). 리틀엔디안이면 뒤집고, 빅엔디안이면 그대로 반환 */
uint64_t htonll(uint64_t n)
{
	return is_little_endian() ? swap64(n) : n;
}

/* network -> host (64bit). htonll 과 같은 연산 */
uint64_t ntohll(uint64_t n)
{
	return is_little_endian() ? swap64(n) : n;
}

/*
 * 검증 1: htonll 결과를 "메모리 바이트 순서"로 봤을 때 빅엔디안(최상위 바이트가 첫 번째)인지 확인.
 *         호스트 엔디안과 무관하게 항상 성립해야 하는 정의 그대로의 검사이다.
 */
static int check_network_order(uint64_t host_val)
{
	uint64_t net = htonll(host_val);
	unsigned char *p = (unsigned char *)&net;
	int k;

	for (k = 0; k < 8; k++) {
		/* k 번째 바이트는 host_val 의 (7 - k) 번째 바이트(최상위부터) 여야 한다 */
		unsigned char expect = (unsigned char)((host_val >> (8 * (7 - k))) & 0xFF);
		if (p[k] != expect) {
			return 0;
		}
	}
	return 1;
}

int main(void)
{
	/* 경계값 위주: 바이트마다 값이 달라서 위치 오류가 드러나는 값, 0, 전부 1, 양 끝 바이트 */
	const uint64_t vectors[] = {
		0x0102030405060708ULL,	/* 바이트마다 다름: 이동량이 틀리면 바로 드러남 */
		0x1234567812345678ULL,
		0x0000000000000000ULL,
		0xFFFFFFFFFFFFFFFFULL,
		0x00000000000000FFULL,	/* 맨 아래 바이트만 */
		0xFF00000000000000ULL,	/* 맨 위 바이트만 */
		0x8000000000000001ULL	/* 부호 비트가 켜진 값: 오른쪽 시프트 때 부호 확장이 없는지 확인 */
	};
	int count = (int)(sizeof(vectors) / sizeof(vectors[0]));
	int i, fail = 0;

	printf("host endian: %s\n", is_little_endian() ? "little" : "big");

	for (i = 0; i < count; i++) {
		uint64_t v   = vectors[i];
		uint64_t net = htonll(v);
		int ok = 1;

		/* 검증 1: 네트워크 바이트 순서(빅엔디안) 맞는지 */
		if (!check_network_order(v)) {
			ok = 0;
		}
		/* 검증 2: 왕복(host -> network -> host) 하면 원래 값 */
		if (ntohll(net) != v) {
			ok = 0;
		}
		/* 검증 3: 컴파일러 내장 bswap64 와 비교 (gcc 전용, 리틀엔디안일 때만 의미 있음) */
		if (is_little_endian() && net != __builtin_bswap64(v)) {
			ok = 0;
		}

		/* uint64_t 는 %llx 가 아니라 PRIx64 로 출력해야 이식성이 있다 */
		printf("[%s] host=0x%016" PRIx64 " -> net=0x%016" PRIx64 "\n", ok ? "PASS" : "FAIL", v, net);
		if (!ok) {
			fail++;
		}
	}

	printf("%s (%d/%d passed)\n", fail ? "FAILED" : "ALL PASSED", count - fail, count);

	return fail ? 1 : 0;
}
