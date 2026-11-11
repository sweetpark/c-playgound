#include <string.h>
#include "mock_session.h"

typedef struct { const char *sid; role_t role; } session_entry_t;
static session_entry_t g_sessions[8];
static int g_session_count = 0;

void mock_session_register(const char *session_id, role_t role)
{
    if (g_session_count < 8) {
        g_sessions[g_session_count].sid = session_id;
        g_sessions[g_session_count].role = role;
        g_session_count++;
    }
}

role_t mock_session_lookup_role(const char *session_id)
{
    int i;
    if (session_id == NULL) return ROLE_GUEST;
    for (i = 0; i < g_session_count; i++) {
        if (strcmp(g_sessions[i].sid, session_id) == 0) return g_sessions[i].role;
    }
    return ROLE_GUEST;
}
