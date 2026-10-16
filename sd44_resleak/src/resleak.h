#ifndef RESLEAK_H
#define RESLEAK_H
#define POOL_SIZE 4
int alloc_handle(void);
void free_handle(int h);
int handles_in_use(void);
void reset_handle_pool(void);
int process_bad(int simulate_late_failure);
int process_good(int simulate_late_failure);
#endif
