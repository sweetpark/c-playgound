# HINT2 (막혔을 때만)
```
void copy_raw_bad(const char *raw, char *out)
{
    /* 1. strcpy(out, raw)   ← raw 가 널로 끝난다고 믿는다 */
}

int copy_raw_good(const char *raw, size_t raw_len, char *out, size_t out_size)
{
    /* [ ] 1. 복사할 길이
     *        n = (raw_len < out_size - 1) ? raw_len : out_size - 1 */

    /* [ ] 2. memcpy(out, raw, n)   ← raw 에는 길이 기반 함수만 쓴다 */

    /* [ ] 3. out[n] = '\0'   ← 널 종료는 out 쪽에서 직접 */

    /* [ ] 4. return (raw_len < out_size) ? 0 : -1   (잘렸으면 -1) */
}
```
