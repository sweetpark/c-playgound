# HINT2 (막혔을 때만)
```
int open_predictable_tmp_bad(const char *dir, const char *content)
{
    /* 1. snprintf(path, sizeof(path), "%s/guess.tmp", dir)   ← 이름이 항상 같다
     * 2. access(path, W_OK) != 0 이면 return -1
     * 3. fopen(path, "w+")   ← 심볼릭 링크면 그대로 따라간다
     * 4. fputs, fclose, return 0 */
}

int create_random_tmp_good(const char *dir, char *out_path, size_t out_size,
                           const char *content)
{
    /* [ ] 1. 템플릿 만들기
     *        snprintf(path, sizeof(path), "%s/tmpXXXXXX", dir)
     *        잘렸으면 → return -1 */

    /* [ ] 2. fd = mkstemp(path)   ← 이름을 커널이 무작위로 채운다
     *        fd == -1 이면 → return -1 */

    /* [ ] 3. f = fdopen(fd, "w")
     *        실패 → close(fd), return -1 */

    /* [ ] 4. fputs(content, f), fclose(f) */

    /* [ ] 5. 결과 경로를 호출자 버퍼로 복사
     *        out_size <= strlen(path) → return -1
     *        snprintf(out_path, out_size, "%s", path) */

    /* [ ] 6. return 0 */
}
```
