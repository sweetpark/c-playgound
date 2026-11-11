# HINT2 (막혔을 때만)
```
void report_error_bad(const char *detail, char *user_msg, size_t user_msg_size)
{
    /* 1. snprintf(user_msg, user_msg_size, "Error: %s", detail)   ← 상세 정보를 그대로 노출 */
}

void report_error_good(const char *detail, char *user_msg, size_t user_msg_size,
                       char *log_msg, size_t log_msg_size)
{
    /* [ ] 1. 매개변수에 log_msg, log_msg_size 를 **추가**한다
     *        (출력 채널이 둘로 분리된다) */

    /* [ ] 2. 사용자 메시지는 고정 문자열
     *        snprintf(user_msg, user_msg_size, "%s", "요청 처리 중 오류가 발생했습니다.") */

    /* [ ] 3. 상세 정보는 로그로만
     *        snprintf(log_msg, log_msg_size, "[internal] %s", detail) */
}
```
