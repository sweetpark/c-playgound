#ifndef DOC_CONVERT_H
#define DOC_CONVERT_H
#include <stddef.h>
int build_convert_command_bad(const char *op, const char *documentName, char *out, size_t out_size);
int build_convert_command_good(const char *op, const char *documentName, char *out, size_t out_size);
#endif
