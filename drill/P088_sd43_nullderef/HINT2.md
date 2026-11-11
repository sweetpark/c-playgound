# HINT2 (막혔을 때만)
```
void copy_env_bad(const char *key, char *out, size_t out_size)
{
    /* 1. p = lookup_env_like(key)
     * 2. strncpy(out, p, out_size - 1)   ← NULL 검사 없이 역참조
     * 3. out[out_size - 1] = '\0' */
}

int copy_env_good(const char *key, char *out, size_t out_size)
{
    /* [ ] 1. p = lookup_env_like(key)
     *        "HOME" 이면 "/home/appuser", 그 외에는 NULL */

    /* [ ] 2. p == NULL 이면 → return -1   ← 사용 전 NULL 검사 */

    /* [ ] 3. strncpy(out, p, out_size - 1) */

    /* [ ] 4. out[out_size - 1] = '\0' */

    /* [ ] 5. return 0 */
}
```
