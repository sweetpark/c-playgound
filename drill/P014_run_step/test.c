#include <errno.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

static int g_cleanups;

static int step_ok(void *ctx)
{
	(void)ctx;
	return 0;
}

static int step_enoent(void *ctx)
{
	(void)ctx;
	errno = ENOENT;
	return -1;
}

static int step_eacces_pos(void *ctx)
{
	(void)ctx;
	errno = EACCES;
	return 1;                 /* 0 이 아니면 모두 실패 */
}

static int step_no_errno(void *ctx)
{
	(void)ctx;
	errno = 0;
	return -1;                /* errno 를 안 남기고 실패 */
}

/* 정리 함수가 errno 를 덮어쓰는 상황 (printf/close/free 가 실제로 그럴 수 있다) */
static void cleanup_clobber(void *ctx)
{
	(void)ctx;
	g_cleanups++;
	errno = EBADF;
}

static void cleanup_count(void *ctx)
{
	int *p = ctx;
	g_cleanups++;
	if (p) (*p)++;
}

int main(void)
{
	int r;
	int hits = 0;

	/* 성공: 정리는 한 번 호출된다 */
	g_cleanups = 0;
	r = run_step(step_ok, cleanup_clobber, NULL);
	CHECK(r == 0);
	CHECK(g_cleanups == 1);

	/* 핵심: 실패 직후 errno 를 저장 → 정리가 덮어써도 원래 값을 돌려준다 */
	g_cleanups = 0;
	errno = 0;
	r = run_step(step_enoent, cleanup_clobber, NULL);
	CHECK(r == ENOENT);
	CHECK(errno == ENOENT);       /* 호출자가 errno 로 봐도 원인이 남아 있다 */
	CHECK(g_cleanups == 1);

	/* 양수 반환도 실패 */
	r = run_step(step_eacces_pos, cleanup_clobber, NULL);
	CHECK(r == EACCES);
	CHECK(errno == EACCES);

	/* errno 가 0 인 실패는 EIO 로 */
	r = run_step(step_no_errno, cleanup_clobber, NULL);
	CHECK(r == EIO);
	CHECK(errno == EIO);

	/* ctx 전달, cleanup == NULL 허용 */
	g_cleanups = 0;
	r = run_step(step_enoent, cleanup_count, &hits);
	CHECK(r == ENOENT);
	CHECK(hits == 1);
	CHECK(run_step(step_ok, NULL, NULL) == 0);
	CHECK(run_step(step_enoent, NULL, NULL) == ENOENT);

	/* step == NULL : EINVAL, cleanup 은 부르지 않는다 */
	g_cleanups = 0;
	r = run_step(NULL, cleanup_clobber, NULL);
	CHECK(r == EINVAL);
	CHECK(g_cleanups == 0);

	return drill_report();
}
