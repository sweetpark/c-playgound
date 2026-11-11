# HINT2 (막혔을 때만)
```
void write_at_index_bad(int *arr, int num, int value)
{
    /* 1. arr[num - 1] = value   ← 하한·상한 검사 없음 */
}

int write_at_index_good(int *arr, size_t arr_size, int num, int value)
{
    /* [ ] 1. 범위 검사 (쓰기 전, 한 줄에 둘 다)
     *        num <= 0 또는 (size_t)num > arr_size → return -1 */

    /* [ ] 2. arr[num - 1] = value */

    /* [ ] 3. return 0 */
}
```
