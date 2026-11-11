#include <string.h>
#include "mock_login.h"
#include "record_access.h"

int view_record_bad(const char *sid, const char *targetUsername)
{
    const char *loginUser = mock_login_lookup_user(sid);
    (void)targetUsername;
    return (loginUser != NULL);
}

int view_record_good(const char *sid, const char *targetUsername)
{
    const char *loginUser = mock_login_lookup_user(sid);
    if (loginUser == NULL) return 0;
    return (strcmp(loginUser, targetUsername) == 0);
}
