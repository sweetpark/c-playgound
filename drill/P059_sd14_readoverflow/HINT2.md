# HINT2 (막혔을 때만)
```
int read_at_index_bad(const int *arr, int idx)
{
    /* 1. return arr[idx]   ← 범위 검사 없음 */
}

int read_at_index_good(const int *arr, size_t size, int idx, int *out)
{
    /* [ ] 1. 범위 검사 (읽기 전)
     *        idx < 0 또는 (size_t)idx >= size → return -1 */

    /* [ ] 2. *out = arr[idx]
     *        값은 out 으로, 상태는 리턴으로 분리한다 */

    /* [ ] 3. return 0 */
}
```
