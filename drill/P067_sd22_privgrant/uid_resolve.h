#ifndef UID_RESOLVE_H
#define UID_RESOLVE_H
typedef enum { GRP_ADMIN, GRP_NORMAL, GRP_GUEST, GRP_UNKNOWN } group_t;
int resolve_uid_bad(int isCorrectUser);
int resolve_uid_good(group_t group);
#endif
