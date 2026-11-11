#ifndef KEY_LOAD_H
#define KEY_LOAD_H
#include <stddef.h>
const unsigned char *get_key_bad(void);
int get_key_good(const char *key_name, unsigned char *out, size_t out_len);
#endif
