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
    int rc = step1_will_fail ? -1 : 0;
    if (rc != 0) {
        return -1;                     /* 실제 조치: 즉시 중단하고 실패를 알림 */
    }
    *out_balance -= 100;
    return 0;
}
