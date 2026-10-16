#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "symmap.h"

int open_predictable_tmp_bad(const char *dir, const char *content)
{
    char path[PATH_MAX];
    FILE *f;
    snprintf(path, sizeof(path), "%s/guess.tmp", dir);   /* 이름이 항상 같다 -> 예측 가능 */

    if (access(path, W_OK) != 0) return -1;
    f = fopen(path, "w+");                                /* 심볼릭 링크면 그대로 따라간다 */
    if (!f) return -1;
    fputs(content, f);
    fclose(f);
    return 0;
}

int create_random_tmp_good(const char *dir, char *out_path, size_t out_size, const char *content)
{
    char path[PATH_MAX];
    int fd;
    FILE *f;

    if (snprintf(path, sizeof(path), "%s/tmpXXXXXX", dir) >= (int)sizeof(path)) return -1;

    fd = mkstemp(path);                                   /* 무작위 접미사 -- 미리 예측 불가 */
    if (fd == -1) return -1;

    f = fdopen(fd, "w");
    if (!f) { close(fd); return -1; }
    fputs(content, f);
    fclose(f);

    if (out_size <= strlen(path)) return -1;
    snprintf(out_path, out_size, "%s", path);
    return 0;
}
