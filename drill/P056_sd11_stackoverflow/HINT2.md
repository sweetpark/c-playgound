# HINT2 (막혔을 때만)
```
void manipulate_string_bad(const char *input)
{
    /* 1. char buf[24]
     * 2. strcpy(buf, input)   ← 길이 검사 없음 */
}

int manipulate_string_good(const char *input, char *out, size_t out_size)
{
    /* [ ] 1. 길이 검사 (복사 전)
     *        strlen(input) >= out_size → return -1 */

    /* [ ] 2. strncpy(out, input, out_size - 1) */

    /* [ ] 3. 널 종료를 **직접** 채운다
     *        out[out_size - 1] = '\0' */

    /* [ ] 4. return 0 */
}
```
