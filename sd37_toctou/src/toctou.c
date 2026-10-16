#define _DEFAULT_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include "toctou.h"

int file_operation_bad(const char *path, const char *content)
{
    FILE *f;
    if (access(path, W_OK) != 0) return -1;      /* 검사 시점(TOC) */
    f = fopen(path, "w+");                        /* 사용 시점(TOU) -- 그 사이 이름이 다른 대상을 가리켜도 그대로 따라감 */
    if (!f) return -1;
    fputs(content, f);
    fclose(f);
    return 0;
}

int file_operation_good(const char *path, const char *content)
{
    int fd = open(path, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR); /* 생성.검사 원자적 */
    FILE *f;
    if (fd == -1) return -1;
    if (fchmod(fd, S_IRUSR | S_IWUSR) == -1) { close(fd); return -1; }
    f = fdopen(fd, "w");
    if (!f) { close(fd); return -1; }
    fputs(content, f);
    fclose(f);
    return 0;
}
