#ifndef SECURE_TRANSMIT_H
#define SECURE_TRANSMIT_H
#include <stddef.h>
void prepare_for_send_bad(const char *pw, char *out, size_t out_size);
void prepare_for_send_good(const char *pw, char *out, size_t out_size);
void undo_mask(const char *masked, char *out, size_t out_size);
#endif
