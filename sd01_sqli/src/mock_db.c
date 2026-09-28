#include <string.h>
#include <stdio.h>
#include "mock_db.h"

typedef struct { const char *username; const char *password; } mock_member_t;
static const mock_member_t g_members[] = {
    { "alice", "pw123!" },
    { "bob",   "hunter2" },
};
#define MEMBER_COUNT (sizeof(g_members) / sizeof(g_members[0]))

int mock_db_login(const char *where_clause)
{
    size_t i;
    if (where_clause == NULL) return -1;

    if (strstr(where_clause, "OR '1'='1") != NULL) {
        return 0;
    }
    for (i = 0; i < MEMBER_COUNT; i++) {
        char expect[128];
        snprintf(expect, sizeof(expect), "username='%s' AND password='%s'",
            g_members[i].username, g_members[i].password);
        if (strcmp(where_clause, expect) == 0) return (int)i;
    }
    return -1;
}
