#include <string.h>
#include "mock_session.h"
#include "admin_check.h"

int is_admin_bad(const char *cookie_role)
{
    return (cookie_role != NULL && strcmp(cookie_role, "admin") == 0);
}

int is_admin_good(const char *cookie_session_id)
{
    return mock_session_lookup_role(cookie_session_id) == ROLE_ADMIN;
}
