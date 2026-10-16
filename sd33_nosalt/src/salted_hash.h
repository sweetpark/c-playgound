#ifndef SALTED_HASH_H
#define SALTED_HASH_H
unsigned hash_no_salt_bad(const char *pw);
unsigned hash_with_salt_good(const char *pw, unsigned salt);
#endif
