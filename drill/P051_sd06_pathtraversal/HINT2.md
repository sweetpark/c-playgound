# HINT2 (막혔을 때만)
```
char *resolve_report_path_bad(const char *sandbox_root, const char *rName,
                              char *out, size_t out_size)
{
    /* 1. 길이만 확인하고 "%s/%s" 로 문자열을 이어 붙인다   ← 정규화 없음
     * 2. return out */
}

int resolve_report_path_good(const char *sandbox_root, const char *rName,
                             char *out, size_t out_size)
{
    /* [ ] 1. 지역변수
     *        joined[PATH_MAX], real_sandbox[PATH_MAX], real_target[PATH_MAX], sandbox_len */

    /* [ ] 2. 문자열 결합
     *        snprintf 결과 >= sizeof(joined) 이면 → return -1 */

    /* [ ] 3. 정규화
     *        realpath(sandbox_root, real_sandbox) 실패 → return -1
     *        realpath(joined, real_target)        실패 → return -1  (없는 파일도 여기서 걸린다) */

    /* [ ] 4. 접두어 비교
     *        sandbox_len = strlen(real_sandbox)
     *        strncmp(real_target, real_sandbox, sandbox_len) != 0 → return -1
     *        real_target[sandbox_len] 가 '/' 도 '\0' 도 아니면    → return -1  (형제 디렉터리 차단) */

    /* [ ] 5. out_size <= strlen(real_target) 이면 → return -1 */

    /* [ ] 6. snprintf(out, out_size, "%s", real_target) 후 return 0 */
}
```
