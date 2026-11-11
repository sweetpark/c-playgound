/* P086 erract — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include "erract.h"

int process_request_bad(int step1_will_fail, int *out_balance)
{
    int rc = step1_will_fail ? -1 : 0;
    if (rc != 0) {
        /* do nothing */              /* 검사는 하지만 아무 조치도 없음 */
    }
    *out_balance -= 100;               /* 실패했든 말든 그대로 진행 */
    return 0;                          /* 실패를 숨기고 항상 성공을 보고 */
}

int process_request_good(int step1_will_fail, int *out_balance)
{
	(void)step1_will_fail;
	(void)out_balance;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

