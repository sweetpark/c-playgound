#include <string.h>
#include "buf_ops.h"

void manipulate_string_bad(const char *input)
{
    char buf[24];
    strcpy(buf, input);      /* 길이 검사 없는 복사 */
    (void)buf;
}

int manipulate_string_good(const char *input, char *out, size_t out_size)
{
    if (strlen(input) >= out_size) return -1;
    strncpy(out, input, out_size - 1);
    out[out_size - 1] = '\0';
    return 0;
}
