#include <stdlib.h>
#include "badfree.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"
void free_stack_addr_bad(void)
{
    char p[10];
    free(p);                          /* 스택 주소 해제 -> 비정상 동작 */
}
#pragma GCC diagnostic pop

int alloc_and_free_good(void)
{
    char *p = (char *)malloc(10);
    if (!p) return -1;
    p[0] = 'x';                       /* 사용 */
    free(p);                          /* malloc으로 받은 포인터만 해제 */
    return 0;
}
