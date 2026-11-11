#include <stdlib.h>
#include <string.h>
#include "heap_ops.h"

#define BUFSIZE 10

void heap_copy_bad(const char *input)
{
    char *dest = (char *)malloc(BUFSIZE);
    if (dest == NULL) return;
    strcpy(dest, input);
    free(dest);
}

int heap_copy_good(const char *input, char **out)
{
    size_t len = strlen(input);
    if (len >= BUFSIZE) return -1;
    *out = (char *)malloc(BUFSIZE);
    if (*out == NULL) return -1;
    strncpy(*out, input, BUFSIZE - 1);
    (*out)[BUFSIZE - 1] = '\0';
    return 0;
}
