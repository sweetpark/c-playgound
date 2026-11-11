/* P101 chrootjail — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <stdio.h>
#include <unistd.h>
#include "chrootjail.h"

int resolve_in_jail_bad(const char *stale_cwd, const char *user_path, char *out, size_t out_size)
{
    /* chroot() 이후에도 chdir("/")를 안 해서, 여전히 원래(jail 밖) 작업 디렉터리 기준으로 해석 */
    if (snprintf(out, out_size, "%s/%s", stale_cwd, user_path) >= (int)out_size) return -1;
    return access(out, F_OK) == 0 ? 0 : -1;
}

int resolve_in_jail_good(const char *jail_root, const char *user_path, char *out, size_t out_size)
{
	(void)jail_root;
	(void)user_path;
	(void)out;
	(void)out_size;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

