#ifndef GETLOGINSIM_H
#define GETLOGINSIM_H
#include <stddef.h>
const char *getlogin_like_bad(const char *name);
int getlogin_like_good_r(const char *name, char *buf, size_t buf_size);
#endif
