#include "key_length.h"
#define MIN_SYMMETRIC_KEY_BITS 128

int validate_key_length_bad(int key_bits) { (void)key_bits; return 1; }
int validate_key_length_good(int key_bits) { return key_bits >= MIN_SYMMETRIC_KEY_BITS; }
