#include <stddef.h>
#include <stdint.h>

/* P029 be64 — 모범답안 */
void put_be64(uint8_t out[8], uint64_t v)
{
	int i;

	for (i = 0; i < 8; i++)
		out[i] = (uint8_t)((v >> (56 - 8 * i)) & 0xFF);   /* 시프트 후 마스크, 바이트별 이동량 56,48,...,0 */
}

uint64_t get_be64(const uint8_t in[8])
{
	uint64_t v = 0;
	int i;

	for (i = 0; i < 8; i++)
		v = (v << 8) | in[i];     /* in[i] 를 먼저 64비트로 올리지 않으면 int 시프트 폭 문제 */
	return v;
}
