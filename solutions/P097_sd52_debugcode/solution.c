#include "debugcode.h"

void debug_output_bad(void)
{
    puts("Debug message");        /* 배포본에 남은 디버그 출력 -- 조건 없이 항상 실행 */
}

void debug_output_good(void)
{
    DEBUG_LOG("Debug message");   /* DEBUG_ENABLE 없이 빌드하면 아무 일도 안 함 */
}
