#include <stdio.h>
#include <string.h>
#include "doc_convert.h"

int build_convert_command_bad(const char *op, const char *documentName, char *out, size_t out_size)
{
    int n = snprintf(out, out_size, "%s %s", op, documentName);
    return (n >= 0 && (size_t)n < out_size) ? 0 : -1;
}

typedef struct { const char *op; const char *command; } op_map_t;
static const op_map_t OP_TABLE[] = {
    { "DocToHtml", "convertDocToHtml" },
    { "DocToPdf",  "convertDocToPdf"  },
};
#define OP_TABLE_SIZE (sizeof(OP_TABLE)/sizeof(OP_TABLE[0]))

int build_convert_command_good(const char *op, const char *documentName, char *out, size_t out_size)
{
    size_t i;
    for (i = 0; i < OP_TABLE_SIZE; i++) {
        if (strcmp(op, OP_TABLE[i].op) == 0) {
            int n = snprintf(out, out_size, "%s %s", OP_TABLE[i].command, documentName);
            return (n >= 0 && (size_t)n < out_size) ? 0 : -1;
        }
    }
    return -1;
}
