#ifndef EXCEPT_H
#define EXCEPT_H
#include <stddef.h>
void copy_input_bad(char *from_buf, size_t from_size, int simulate_read_fail,
                     char *to_buf, size_t to_size);
int copy_input_good(char *from_buf, size_t from_size, int simulate_read_fail,
                     char *to_buf, size_t to_size);
#endif
