#include <string.h>
#include "mock_keystore.h"

typedef struct { const char *name; const unsigned char *key; size_t len; } entry_t;
static entry_t g_keys[8];
static int g_count = 0;

void mock_keystore_register(const char *key_name, const unsigned char *key, size_t key_len)
{
    if (g_count < 8) {
        g_keys[g_count].name = key_name;
        g_keys[g_count].key = key;
        g_keys[g_count].len = key_len;
        g_count++;
    }
}

int mock_keystore_load(const char *key_name, unsigned char *out, size_t out_len)
{
    int i;
    for (i = 0; i < g_count; i++) {
        if (strcmp(g_keys[i].name, key_name) == 0) {
            if (g_keys[i].len > out_len) return -1;
            memcpy(out, g_keys[i].key, g_keys[i].len);
            return 0;
        }
    }
    return -1;
}
