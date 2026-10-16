#ifndef INT_ALLOC_H
#define INT_ALLOC_H
#include <stddef.h>
void *int_alloc_bad(int count);
void *int_alloc_good(int count);
void use_buffer_bad(int count);
void *use_buffer_good(int count);
#endif
