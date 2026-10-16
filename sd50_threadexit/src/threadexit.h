#ifndef THREADEXIT_H
#define THREADEXIT_H
int unreaped_count(void);
void reset_unreaped(void);
int spawn_worker_bad(int *result);
int spawn_worker_good(int *result);
#endif
