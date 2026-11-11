/* P041 cfg — 요구사항은 problem.md. 헤더(*.h)의 선언을 보고 함수를 채운다. */
#include <ctype.h>
#include <limits.h>
#include "common.h"
#include "log.h"
#include "safe.h"
#include "guard.h"
#include "test_alloc.h"
#include "cfg.h"

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

/* ── 생성: 소유권을 호출부로 넘긴다 ─────────────────────────── */
int cfg_create(const char *path, cfg_t **out_cfg)
{
	(void)path;
	(void)out_cfg;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

void cfg_destroy(cfg_t *cfg)
{
	(void)cfg;
	/* TODO: problem.md 를 보고 Good 구현 */
	drill_todo_hit = 1; /* 구현하면 이 줄을 지운다 */
}

int cfg_get_str(const cfg_t *cfg, const char *key, const char **out)
{
	(void)cfg;
	(void)key;
	(void)out;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

int cfg_get_int(const cfg_t *cfg, const char *key, int *out)
{
	(void)cfg;
	(void)key;
	(void)out;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

