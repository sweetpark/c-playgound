#ifndef TOKEN_GEN_H
#define TOKEN_GEN_H
#include <stddef.h>
void gen_token_bad(unsigned seed, char *out, size_t out_size);
int gen_token_good(char *out, size_t out_size);
#endif
