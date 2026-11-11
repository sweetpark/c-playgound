# HINT2 (막혔을 때만)
```
int lookup_info_bad(int id, const int *info)
{
    /* 1. short s = (short)id      ← 축소 변환, 음수화 가능
     * 2. s > 256 이면 return -1   ← 음수는 통과
     * 3. unsigned sz = (unsigned)s ← 음수 → 거대한 값
     * 4. return info[sz] */
}

int lookup_info_good(int id, const int *info, size_t info_size)
{
    /* [ ] 1. 원본 int 로 직접 범위 검사 (축소 변환 없이)
     *        id < 0 또는 (size_t)id >= info_size → return -1 */

    /* [ ] 2. return info[id] */
}
```
