#ifndef ERRMSG_H
#define ERRMSG_H
#include <stddef.h>
void report_error_bad(const char *detail, char *user_msg, size_t user_msg_size);
void report_error_good(const char *detail, char *user_msg, size_t user_msg_size,
                        char *log_msg, size_t log_msg_size);
#endif
