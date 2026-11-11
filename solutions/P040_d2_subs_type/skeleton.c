/* P040 subs_type — 요구사항은 problem.md. 헤더(*.h)의 선언을 보고 함수를 채운다. */
#include "common.h"
#include "log.h"
#include "guard.h"
#include "subs_type.h"

/* 인덱스 = enum 값. 순서가 어긋나면 전부 틀어지므로 한 곳에서만 관리한다 */

const char *rat_type_to_str(rat_type_t v)
{
	(void)v;
	/* TODO: problem.md 를 보고 Good 구현 */
	return NULL;
}

int rat_type_from_str(const char *s, rat_type_t *out)
{
	(void)s;
	(void)out;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

void subs_profile_init(subs_profile_t *p)
{
	(void)p;
	/* TODO: problem.md 를 보고 Good 구현 */
	drill_todo_hit = 1; /* 구현하면 이 줄을 지운다 */
}

#define V_FAIL(_fmt, ...)                                               \
    do { LOG_ERR("validate: " _fmt, __VA_ARGS__); return RET_BAD_FORMAT; } while (0)

int subs_profile_validate(const subs_profile_t *p)
{
	(void)p;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

const prepaid_info_t *subs_profile_get_prepaid(const subs_profile_t *p)
{
	(void)p;
	/* TODO: problem.md 를 보고 Good 구현 */
	return NULL;
}

const postpaid_info_t *subs_profile_get_postpaid(const subs_profile_t *p)
{
	(void)p;
	/* TODO: problem.md 를 보고 Good 구현 */
	return NULL;
}

int subs_profile_set_prepaid_balance(subs_profile_t *p, uint32_t balance)
{
	(void)p;
	(void)balance;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

int subs_profile_set_postpaid(subs_profile_t *p, const postpaid_info_t *src)
{
	(void)p;
	(void)src;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

void subs_profile_dump(const subs_profile_t *p)
{
	(void)p;
	/* TODO: problem.md 를 보고 Good 구현 */
	drill_todo_hit = 1; /* 구현하면 이 줄을 지운다 */
}

