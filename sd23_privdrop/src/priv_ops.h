#ifndef PRIV_OPS_H
#define PRIV_OPS_H
int get_current_priv(void);
void reset_priv(void);
void elevate_bad(void);
void do_privileged_work_good(void);
#endif
