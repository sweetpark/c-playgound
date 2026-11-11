#ifndef DNSTRUST_H
#define DNSTRUST_H
#include <stddef.h>
const char *resolve_hostname_like(const char *hostname, int poisoned);
int decide_send_target_bad(const char *hostname, int poisoned, char *out_ip, size_t out_size);
int decide_send_target_good(const char *claimed_ip, char *out_ip, size_t out_size);
#endif
