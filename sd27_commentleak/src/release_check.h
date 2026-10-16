#ifndef RELEASE_CHECK_H
#define RELEASE_CHECK_H
int find_secret_in_comments(const char *source_code);
int release_ok_bad(const char *source_code);
int release_ok_good(const char *source_code);
#endif
