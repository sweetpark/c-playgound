/* P075 keylen — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include "key_length.h"
#define MIN_SYMMETRIC_KEY_BITS 128

int validate_key_length_bad(int key_bits) { (void)key_bits; return 1; }
int validate_key_length_good(int key_bits)
{
	(void)key_bits;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

