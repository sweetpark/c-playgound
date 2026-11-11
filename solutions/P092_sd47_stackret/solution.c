#include <string.h>
#include <stdlib.h>
#include "stackret.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wreturn-local-addr"
#pragma GCC diagnostic ignored "-Wdangling-pointer"
char *rpl_bad(void)
{
    char p[10];
    strcpy(p, "SECRET123");
    return p;                    /* 스택 지역 배열의 주소 반환 -- 함수 종료 시 소멸 */
}
#pragma GCC diagnostic pop

char *rpl_good(void)
{
    char p[10];
    char *buf = (char *)malloc(10);
    if (!buf) return NULL;
    strcpy(p, "SECRET123");
    memcpy(buf, p, 10);
    return buf;                  /* 힙에 있으므로 함수 종료 후에도 유효 -- 호출자가 free */
}
