/* P085 errmsg — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <stdio.h>
#include "errmsg.h"

void report_error_bad(const char *detail, char *user_msg, size_t user_msg_size)
{
    snprintf(user_msg, user_msg_size, "Error: %s", detail);   /* 상세 정보를 그대로 노출 */
}

void report_error_good(const char *detail, char *user_msg, size_t user_msg_size,
                        char *log_msg, size_t log_msg_size)
{
	(void)detail;
	(void)user_msg;
	(void)user_msg_size;
	(void)log_msg;
	(void)log_msg_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	drill_todo_hit = 1; /* 구현하면 이 줄을 지운다 */
}

