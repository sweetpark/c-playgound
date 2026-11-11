/* P097 debugcode — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include "debugcode.h"

void debug_output_bad(void)
{
    puts("Debug message");        /* 배포본에 남은 디버그 출력 -- 조건 없이 항상 실행 */
}

void debug_output_good(void)
{
	/* TODO: problem.md 를 보고 Good 구현 */
	drill_todo_hit = 1; /* 구현하면 이 줄을 지운다 */
}

