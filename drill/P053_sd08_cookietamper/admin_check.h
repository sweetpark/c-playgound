#ifndef ADMIN_CHECK_H
#define ADMIN_CHECK_H
int is_admin_bad(const char *cookie_role);
int is_admin_good(const char *cookie_session_id);
#endif
