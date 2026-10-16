#include "padding_check.h"

int is_safe_padding_bad(padding_t p) { (void)p; return 1; }
int is_safe_padding_good(padding_t p) { return p != PAD_NONE; }
