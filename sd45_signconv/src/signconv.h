#ifndef SIGNCONV_H
#define SIGNCONV_H
unsigned int len_bad(const char *s);
unsigned int len_good(const char *s);
int within_len_bad(unsigned int idx, const char *s);
int within_len_good(unsigned int idx, const char *s);
#endif
