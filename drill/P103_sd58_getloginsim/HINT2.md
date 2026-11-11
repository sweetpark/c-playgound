# HINT2 (막혔을 때만)
```
const char *getlogin_like_bad(const char *name)
{
    /* 1. snprintf(g_shared_buf, sizeof(g_shared_buf), "%s", name)   ← 정적 버퍼
     * 2. return g_shared_buf   ← 스레드 간 공유 */
}

int getlogin_like_good_r(const char *name, char *buf, size_t buf_size)
{
    /* [ ] 1. 반환형을 포인터 → int(상태코드) 로 바꾼다
     *        결과는 **호출자 소유 buf** 에 쓴다 */

    /* [ ] 2. snprintf(buf, buf_size, "%s", name)
     *        잘렸으면 (>= buf_size) → return -1 */

    /* [ ] 3. return 0 */
}
```
