#ifndef HTML_ESCAPE_H
#define HTML_ESCAPE_H
#include <stddef.h>
void render_title_bad(const char *title, char *out, size_t out_size);
void render_title_good(const char *title, char *out, size_t out_size);
#endif
