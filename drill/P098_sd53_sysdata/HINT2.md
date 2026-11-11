# HINT2 (막혔을 때만)
```
void report_exe_not_found_bad(const char *path_env, char *out, size_t out_size)
{
    /* 1. snprintf(out, out_size, "cannot find exe on path %s\n", path_env)   ← 경로 노출 */
}

void report_alloc_failure_bad(void *ptr, char *out, size_t out_size)
{
    /* 1. snprintf(out, out_size, "malloc failed at address %p\n", ptr)   ← 주소 노출 */
}

void report_exe_not_found_good(const char *path_env, char *out, size_t out_size)
{
    /* [ ] 1. (void)path_env   ← 값을 메시지 조합에 **들이지 않는다** */

    /* [ ] 2. snprintf(out, out_size, "cannot find exe on path \n") */
}

void report_alloc_failure_good(void *ptr, char *out, size_t out_size)
{
    /* [ ] 3. (void)ptr */

    /* [ ] 4. snprintf(out, out_size, "malloc failed\n") */
}
```
