#ifndef TOCTOU_H
#define TOCTOU_H
int file_operation_bad(const char *path, const char *content);
int file_operation_good(const char *path, const char *content);
#endif
