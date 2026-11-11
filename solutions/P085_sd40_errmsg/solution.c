#include <stdio.h>
#include "errmsg.h"

void report_error_bad(const char *detail, char *user_msg, size_t user_msg_size)
{
    snprintf(user_msg, user_msg_size, "Error: %s", detail);   /* 상세 정보를 그대로 노출 */
}

void report_error_good(const char *detail, char *user_msg, size_t user_msg_size,
                        char *log_msg, size_t log_msg_size)
{
    snprintf(user_msg, user_msg_size, "%s", "요청 처리 중 오류가 발생했습니다.");
    snprintf(log_msg, log_msg_size, "[internal] %s", detail);  /* 상세 정보는 로그로만 */
}
