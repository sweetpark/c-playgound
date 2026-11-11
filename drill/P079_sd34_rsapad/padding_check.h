#ifndef PADDING_CHECK_H
#define PADDING_CHECK_H
typedef enum { PAD_NONE, PAD_OAEP, PAD_PKCS1 } padding_t;
int is_safe_padding_bad(padding_t p);
int is_safe_padding_good(padding_t p);
#endif
