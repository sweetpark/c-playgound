#include "recur.h"

#define MAX_DEPTH 10000

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Winfinite-recursion"
long fac_bad(int n)
{
    return n * fac_bad(n - 1);      /* 종료 조건이 없어 무한 재귀 -> 스택 고갈 */
}
#pragma GCC diagnostic pop

static long fac_good_impl(int n, int depth, int *ok)
{
    if (depth > MAX_DEPTH) { *ok = 0; return 0; }
    if (n <= 0) { *ok = 1; return 1; }
    {
        int inner_ok = 0;
        long r = fac_good_impl(n - 1, depth + 1, &inner_ok);
        *ok = inner_ok;
        if (!inner_ok) return 0;
        return n * r;
    }
}

int fac_good(int n, long *out)
{
    int ok = 0;
    long r = fac_good_impl(n, 0, &ok);
    if (!ok) return -1;
    *out = r;
    return 0;
}
