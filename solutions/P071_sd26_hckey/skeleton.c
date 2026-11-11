/* P071 hckey — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include "mock_keystore.h"
#include "key_load.h"

static const unsigned char HARD_KEY[16] = {
    0x13,0x2a,0x7f,0xc4,0x09,0xe1,0x55,0xb8,
    0x6d,0x30,0xa2,0x4e,0xf7,0x81,0x0c,0x93
};

const unsigned char *get_key_bad(void)
{
    return HARD_KEY;
}

int get_key_good(const char *key_name, unsigned char *out, size_t out_len)
{
	(void)key_name;
	(void)out;
	(void)out_len;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

