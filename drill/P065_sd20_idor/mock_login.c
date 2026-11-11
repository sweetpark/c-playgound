#include <string.h>
#include "mock_login.h"

typedef struct { const char *sid; const char *username; } entry_t;
static entry_t g_logins[8];
static int g_count = 0;

void mock_login_register(const char *sid, const char *username)
{
    if (g_count < 8) {
        g_logins[g_count].sid = sid;
        g_logins[g_count].username = username;
        g_count++;
    }
}

const char *mock_login_lookup_user(const char *sid)
{
    int i;
    if (sid == NULL) return NULL;
    for (i = 0; i < g_count; i++)
        if (strcmp(g_logins[i].sid, sid) == 0) return g_logins[i].username;
    return NULL;
}
