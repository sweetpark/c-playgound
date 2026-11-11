#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "guard.h"
#include "log.h"
#include "safe.h"

/* P038 d0_warmup — 모범답안 (d0_warmup/main.c 의 take_ptr / take_res) */

static int take_ptr(const char *s, int v)
{
	CHK_PTR(s);
	CHK_RANGE(v, 0, 100);
	LOG_INF("ok: %s / %d", s, v);
	return RET_OK;
}

static int take_res(const char *path)
{
	int ret = RET_FAIL;
	char *buf = NULL;
	FILE *fp = NULL;

	CHK_PTR(path);

	buf = calloc(1, 64);
	CHK_PTR_GOTO(buf, ret, CLEANUP);

	fp = fopen(path, "r");
	CHK_COND_GOTO(fp != NULL, RET_IO_ERROR, ret, CLEANUP);

	CHK_RET_GOTO(take_ptr("inner", 10), ret, CLEANUP);
	ret = RET_OK;

CLEANUP:
	SAFE_FCLOSE(fp);
	SAFE_FREE(buf);
	return ret;
}
