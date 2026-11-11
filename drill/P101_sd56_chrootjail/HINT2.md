# HINT2 (막혔을 때만)
```
int resolve_in_jail_bad(const char *stale_cwd, const char *user_path,
                        char *out, size_t out_size)
{
    /* 1. snprintf(out, out_size, "%s/%s", stale_cwd, user_path)
     *    ← chdir("/") 를 안 해서 jail 밖의 옛 작업 디렉터리가 기준
     * 2. access(out, F_OK) == 0 이면 0, 아니면 -1 */
}

int resolve_in_jail_good(const char *jail_root, const char *user_path,
                         char *out, size_t out_size)
{
    /* [ ] 1. 기준점을 jail 루트로 확정한다
     *        (실제 코드에서는 chroot(...) 직후 chdir("/")) */

    /* [ ] 2. snprintf(out, out_size, "%s/%s", jail_root, user_path)
     *        잘렸으면 (>= out_size) → return -1 */

    /* [ ] 3. return access(out, F_OK) == 0 ? 0 : -1 */
}
```
