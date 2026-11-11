# HINT2 (막혔을 때만)
```
void copy_input_bad(char *from_buf, size_t from_size, int simulate_read_fail,
                    char *to_buf, size_t to_size)
{
    /* 1. mock_fgets(from_buf, from_size, simulate_read_fail)   ← 반환값을 검사하지 않는다
     * 2. strcpy(to_buf, from_buf)   ← 널 종료가 안 되면 오버플로우 */
}

int copy_input_good(char *from_buf, size_t from_size, int simulate_read_fail,
                    char *to_buf, size_t to_size)
{
    /* [ ] 1. mock_fgets 를 먼저 만든다
     *        simulate_fail 이면 buf 를 'A' 로 가득 채우고(널 없음) return NULL
     *        아니면 "hi" 를 복사하고 return buf */

    /* [ ] 2. char *ret = mock_fgets(from_buf, from_size, simulate_read_fail) */

    /* [ ] 3. 반환값 검사
     *        ret != from_buf → return -1 */

    /* [ ] 4. 길이 검사
     *        to_size <= strlen(from_buf) → return -1 */

    /* [ ] 5. strcpy(to_buf, from_buf) */

    /* [ ] 6. return 0 */
}
```
