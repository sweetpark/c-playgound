# HINT2 (막혔을 때만)
```
void chunk_copy_bad(void *dst, const void *src, int initialized, int chunk_size)
{
    /* 1. unsigned size = (unsigned)get_chunk_size(initialized, chunk_size)
     *    ← -1 이 4294967295 로 바뀐다
     * 2. memcpy(dst, src, size) */
}

int chunk_copy_good(void *dst, size_t dst_size, const void *src,
                    int initialized, int chunk_size)
{
    /* [ ] 1. signed 로 받는다
     *        int size = get_chunk_size(initialized, chunk_size) */

    /* [ ] 2. 에러 먼저 확인
     *        size < 0 → return -1 */

    /* [ ] 3. 목적지 크기 검사
     *        (size_t)size > dst_size → return -1 */

    /* [ ] 4. memcpy(dst, src, (size_t)size) */

    /* [ ] 5. return 0 */
}
```
