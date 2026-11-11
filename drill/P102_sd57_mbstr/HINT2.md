# HINT2 (막혔을 때만)
```
void mb_copy_bad(char *dst, const char *src)
{
    /* 1. strcpy(dst, src)   ← 크기 인자가 없다 */
}

int mb_copy_good(char *dst, size_t dst_size, const char *src)
{
    /* [ ] 1. 지역변수
     *        size_t len = strlen(src), copy_len */

    /* [ ] 2. dst_size == 0 이면 → return -1 */

    /* [ ] 3. 복사할 길이
     *        copy_len = (len < dst_size - 1) ? len : dst_size - 1 */

    /* [ ] 4. 멀티바이트 경계 뒤로 물러난다 (복사 **전에**)
     *        copy_len > 0 이고 ((unsigned char)src[copy_len] & 0xC0) == 0x80 인 동안
     *            copy_len-- */

    /* [ ] 5. memcpy(dst, src, copy_len) */

    /* [ ] 6. dst[copy_len] = '\0' */

    /* [ ] 7. return 0 */
}
```
