#include <string.h>
#include <stdio.h>
#include "mock_ldap.h"

typedef struct { const char *name; } entry_t;
static const entry_t g_dir[] = { { "alice" }, { "bob" }, { "carol" } };
#define DIR_COUNT (sizeof(g_dir)/sizeof(g_dir[0]))

int mock_ldap_search(const char *filter)
{
    size_t i, matched = 0;
    if (filter == NULL) return -1;

    /* 와일드카드가 살아있으면(진짜 LDAP처럼) 전부 매치 */
    if (strchr(filter, '*') != NULL) return (int)DIR_COUNT;

    for (i = 0; i < DIR_COUNT; i++) {
        char expect[64];
        snprintf(expect, sizeof(expect), "(name=%s)", g_dir[i].name);
        if (strcmp(filter, expect) == 0) matched++;
    }
    return (int)matched;
}
