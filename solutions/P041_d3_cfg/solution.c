#include <ctype.h>
#include <limits.h>
#include "common.h"
#include "log.h"
#include "safe.h"
#include "guard.h"
#include "test_alloc.h"
#include "cfg.h"

static char *trim(char *s)
{
    char *e = NULL;

    while (*s != '\0' && isspace((unsigned char)*s)) s++;
    e = s + strlen(s);
    while (e > s && isspace((unsigned char)e[-1])) e--;
    *e = '\0';
    return s;
}

#define CFG_KEY_MAX    31
#define CFG_VAL_MAX    127
#define CFG_LINE_MAX   255

typedef struct { char key[CFG_KEY_MAX + 1]; char val[CFG_VAL_MAX + 1]; } cfg_item_t;

struct cfg_s {                       /* 불투명: 헤더에는 typedef 만 노출 */
    cfg_item_t *items;
    size_t      num;
    size_t      cap;
};

/* ── 항목 추가: 중복 키는 덮어쓰기, 모자라면 2배 확장 ───────── */
static int cfg_put(cfg_t *cfg, const char *key, const char *val)
{
    size_t      i   = 0;
    cfg_item_t *tmp = NULL;

    CHK_PTR(cfg); CHK_PTR(key); CHK_PTR(val);

    for (i = 0; i < cfg->num; i++) {                 /* 나중 것이 이긴다 */
        if (strcmp(cfg->items[i].key, key) == 0) {
            snprintf(cfg->items[i].val, sizeof(cfg->items[i].val), "%s", val);
            return RET_OK;
        }
    }

    if (cfg->num == cfg->cap) {
        size_t new_cap = (cfg->cap == 0) ? 8 : cfg->cap * 2;

        tmp = realloc(cfg->items, new_cap * sizeof(*tmp));   /* ★ tmp 로 받는다 */
        if (tmp == NULL) {                                   /*   원본은 그대로 살아있다 */
            LOG_ERR("realloc %zu failed", new_cap);
            return RET_NO_MEM;
        }
        cfg->items = tmp;
        cfg->cap   = new_cap;
    }

    snprintf(cfg->items[cfg->num].key, CFG_KEY_MAX + 1, "%s", key);
    snprintf(cfg->items[cfg->num].val, CFG_VAL_MAX + 1, "%s", val);
    cfg->num++;
    return RET_OK;
}

/* ── 생성: 소유권을 호출부로 넘긴다 ─────────────────────────── */
int cfg_create(const char *path, cfg_t **out_cfg)
{
    int    ret  = RET_FAIL;          /* ① */
    cfg_t *cfg  = NULL;              /* ② */
    FILE  *fp   = NULL;
    char  *line = NULL;

    CHK_PTR(path);                   /* ③ 자원 잡기 전 → 즉시 return */
    CHK_PTR(out_cfg);
    *out_cfg = NULL;                 /*    실패해도 호출부가 쓰레기를 안 보게 */

    cfg = CALLOC(1, sizeof(*cfg));   /* ④ 여기부터 goto */
    CHK_ALLOC_GOTO(cfg, ret, CLEANUP);

    line = CALLOC(1, CFG_LINE_MAX + 2);
    CHK_ALLOC_GOTO(line, ret, CLEANUP);

    fp = fopen(path, "r");
    CHK_COND_GOTO(fp != NULL, RET_IO_ERROR, ret, CLEANUP);

    while (fgets(line, CFG_LINE_MAX + 2, fp) != NULL) {
        char *eq = NULL, *key = NULL, *val = NULL;

        line[strcspn(line, "\r\n")] = '\0';          /* 개행 제거 */
        key = trim(line);
        if (key[0] == '\0' || key[0] == '#') continue;   /* 빈 줄 / 주석 */

        eq = strchr(key, '=');
        CHK_COND_GOTO(eq != NULL, RET_BAD_FORMAT, ret, CLEANUP);

        *eq = '\0';
        val = trim(eq + 1);
        key = trim(key);

        CHK_COND_GOTO(key[0] != '\0', RET_BAD_FORMAT, ret, CLEANUP);
        CHK_COND_GOTO(strlen(key) <= CFG_KEY_MAX, RET_BAD_FORMAT, ret, CLEANUP);
        CHK_COND_GOTO(strlen(val) <= CFG_VAL_MAX, RET_BAD_FORMAT, ret, CLEANUP);

        CHK_RET_GOTO(cfg_put(cfg, key, val), ret, CLEANUP);
    }

    /* 필수 키 검사 */
    {
        int port = 0, worker = 0;
        CHK_RET_GOTO(cfg_get_int(cfg, "listen_port", &port), ret, CLEANUP);
        CHK_RET_GOTO(cfg_get_int(cfg, "worker_num",  &worker), ret, CLEANUP);
        CHK_COND_GOTO(port   >= 1 && port   <= 65535, RET_BAD_FORMAT, ret, CLEANUP);
        CHK_COND_GOTO(worker >= 1 && worker <= 64,    RET_BAD_FORMAT, ret, CLEANUP);
    }

    *out_cfg = cfg;                  /* ★ 소유권 이전 */
    cfg      = NULL;                 /* ★ 아래 CLEANUP 이 해제하지 않도록 끊는다 */
    ret      = RET_OK;               /* ⑤ */

CLEANUP:                             /* ⑥ 잡은 역순 */
    SAFE_FCLOSE(fp);
    SAFE_FREE(line);
    cfg_destroy(cfg);                /*   성공 시엔 NULL 이므로 아무 일도 안 한다 */
    return ret;
}

void cfg_destroy(cfg_t *cfg)
{
    if (cfg == NULL) return;         /* ★ NULL 안전 */
    SAFE_FREE(cfg->items);
    free(cfg);                       /*   객체 자체도 반드시 */
}

int cfg_get_str(const cfg_t *cfg, const char *key, const char **out)
{
    size_t i = 0;

    CHK_PTR(cfg);
    CHK_PTR(key);
    CHK_PTR(out);

    for (i = 0; i < cfg->num; i++) {
        if (strcmp(cfg->items[i].key, key) == 0) { *out = cfg->items[i].val; return RET_OK; }
    }
    return RET_NOT_FOUND;
}

int cfg_get_int(const cfg_t *cfg, const char *key, int *out)
{
    const char *s   = NULL;
    char       *end = NULL;
    long        v   = 0;

    CHK_PTR(out);
    CHK_RET(cfg_get_str(cfg, key, &s));

    errno = 0;
    v = strtol(s, &end, 10);
    if (end == s || *end != '\0' || errno != 0 || v < INT_MIN || v > INT_MAX) {
        LOG_ERR("%s is not an integer: %s", key, s);
        return RET_BAD_FORMAT;
    }
    *out = (int)v;
    return RET_OK;
}
