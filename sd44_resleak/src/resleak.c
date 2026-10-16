#include "resleak.h"

static int g_used[POOL_SIZE];

int alloc_handle(void)
{
    int i;
    for (i = 0; i < POOL_SIZE; i++) {
        if (!g_used[i]) { g_used[i] = 1; return i; }
    }
    return -1;
}

void free_handle(int h)
{
    if (h >= 0 && h < POOL_SIZE) g_used[h] = 0;
}

int handles_in_use(void)
{
    int i, n = 0;
    for (i = 0; i < POOL_SIZE; i++) n += g_used[i];
    return n;
}

void reset_handle_pool(void)
{
    int i;
    for (i = 0; i < POOL_SIZE; i++) g_used[i] = 0;
}

int process_bad(int simulate_late_failure)
{
    int env_hd = alloc_handle();
    int con_hd;
    if (env_hd < 0) return -1;

    con_hd = alloc_handle();
    if (con_hd < 0) return -1;

    if (simulate_late_failure) {
        return -1;                              /* 핸들을 반환하지 않고 함수 종료 -> 자원 누수 */
    }

    free_handle(con_hd);
    free_handle(env_hd);
    return 0;
}

int process_good(int simulate_late_failure)
{
    int env_hd = alloc_handle();
    int con_hd;
    if (env_hd < 0) return -1;

    con_hd = alloc_handle();
    if (con_hd < 0) { free_handle(env_hd); return -1; }

    if (simulate_late_failure) {
        free_handle(con_hd);
        free_handle(env_hd);
        return -1;
    }

    free_handle(con_hd);
    free_handle(env_hd);
    return 0;
}
