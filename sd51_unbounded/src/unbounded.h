#ifndef UNBOUNDED_H
#define UNBOUNDED_H
#define POOL_MAX 4
int active_count(void);
void reset_active(void);
int handle_connection_bad(void);
int handle_connection_good(void);
#endif
