#include <string.h>
#include <stdio.h>
#include "mock_ldap2.h"

typedef struct { const char *name; const char *manager; const char *ou; } entry_t;
static const entry_t g_dir[] = {
    { "alice", "m1", "NewHires" },
    { "bob",   "m1", "NewHires" },
    { "carol", "m1", "Finance"  },  /* 다른 부서 — base 스코프가 지켜지면 노출되면 안 된다 */
};
#define DIR_COUNT (sizeof(g_dir)/sizeof(g_dir[0]))

int mock_ldap_search_base(const char *base, const char *fixed_filter)
{
    size_t i, matched = 0;
    int scope_bypassed;

    if (base == NULL || fixed_filter == NULL) return -1;

    /* base에 와일드카드가 섞이면 스코프가 무력화되어 필터 하나로 전체를 훑는다 */
    scope_bypassed = (strchr(base, '*') != NULL);

    for (i = 0; i < DIR_COUNT; i++) {
        char expect[64];

        if (!scope_bypassed && strcmp(base, "ou=NewHires") != 0) {
            /* mock에서 유효한 고정 base는 "ou=NewHires" 하나뿐이다 */
            continue;
        }
        if (!scope_bypassed && strcmp(g_dir[i].ou, "NewHires") != 0) continue;

        snprintf(expect, sizeof(expect), "manager=%s", g_dir[i].manager);
        if (strcmp(fixed_filter, expect) == 0) matched++;
    }
    return (int)matched;
}
