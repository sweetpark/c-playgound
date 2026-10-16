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
    /* chroot() 직후 chdir("/")까지 해서, 새 루트 자신이 상대 경로의 기준점이 됨 */
    if (snprintf(out, out_size, "%s/%s", jail_root, user_path) >= (int)out_size) return -1;
    return access(out, F_OK) == 0 ? 0 : -1;
}
