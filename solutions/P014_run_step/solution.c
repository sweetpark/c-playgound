#include <errno.h>
#include <stddef.h>

/* P014 run_step — 모범답안 */
int run_step(int (*step)(void *), void (*cleanup)(void *), void *ctx)
{
	int rc = 0;
	int saved;

	if (step == NULL)
		return EINVAL;

	if (step(ctx) != 0) {
		saved = errno;                 /* 실패 직후 바로 저장 */
		rc = (saved != 0) ? saved : EIO;
	}

	if (cleanup != NULL)
		cleanup(ctx);                  /* errno 를 바꿀 수 있다 */

	if (rc != 0)
		errno = rc;                    /* 저장한 값으로 복원 */
	return rc;
}
