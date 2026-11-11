#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "guard.h"
#include "log.h"
#include "safe.h"

/* P038 d0_warmup — 요구사항은 problem.md.  common/ 의 가드·정리 매크로를 쓴다. */

static int take_ptr(const char *s, int v)
{
	(void)s;
	(void)v;
	/* TODO */
	return RET_FAIL;
}

static int take_res(const char *path)
{
	(void)path;
	/* TODO */
	return RET_FAIL;
}
