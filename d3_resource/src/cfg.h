#ifndef CFG_H
#define CFG_H

typedef struct cfg_s cfg_t;

int  cfg_create(const char *path, cfg_t **out_cfg);
void cfg_destroy(cfg_t *cfg);
int  cfg_get_str(const cfg_t *cfg, const char *key, const char **out);
int  cfg_get_int(const cfg_t *cfg, const char *key, int *out);

#endif
