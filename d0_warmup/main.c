#include<stdio.h>
#include<stdlib.h>

#include "common.h"
#include "guard.h"
#include "log.h"
#include "safe.h"

log_level_t g_log_level = LOG_LV_INF;


static int take_ptr(const char *s, int v){
    CHK_PTR(s);
    CHK_RANGE(v, 0, 100);
    LOG_INF("ok: %s / %d", s, v);
    return RET_OK;
}


static int take_res(const char *path){
    int ret = RET_FAIL;
    char *buf = NULL;
    FILE *fp = NULL;

    CHK_PTR(path);

    buf = calloc(1, 64);
    CHK_PTR_GOTO(buf, CLEANUP);

    fp = fopen(path, "r");
    CHK_COND_GOTO(fp != NULL, RET_IO_ERROR, CLEANUP);

    CHK_RET_GOTO(take_ptr("inner", 10), CLEANUP);
    ret = RET_OK;

CLEANUP:
    SAFE_FCLOSE(fp);
    SAFE_FREE(buf);
    return ret;
}

int main(void)
{
    int ret = 0;

    ret = take_ptr(NULL, 10);
    LOG_INF("case1 = %s(%d)", ret_str(ret), ret);

    ret = take_ptr("hi", 999);
    LOG_INF("case2 = %s(%d)", ret_str(ret), ret);

    ret = take_res("/etc/hosts");
    LOG_INF("case3 = %s(%d)", ret_str(ret), ret);

    ret = take_res("/no/such/file");
    LOG_INF("case4 = %s(%d)", ret_str(ret), ret);

    return 0;
}
