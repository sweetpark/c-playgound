# HINT2 (막혔을 때만)
```
void write_before_start_bad(int count)
{
    /* 1. int words[MAX_WORDS]   (MAX_WORDS 10)
     * 2. wordIndex = MAX_WORDS - 1
     * 3. count 번 반복: words[wordIndex] = i, wordIndex--
     *    ← 0 아래로 내려가는지 검사 없음 */
}

int write_before_start_good(int count, int *out_arr, size_t arr_size)
{
    /* [ ] 1. 지역변수
     *        int wordIndex, i */

    /* [ ] 2. count < 0 이면 → return -1 */

    /* [ ] 3. wordIndex = (int)arr_size - 1   (초기값은 항상 안전) */

    /* [ ] 4. count 번 반복
     *        [ ] 4-1. wordIndex < 0 이면 → return -1   ← 매 반복 **쓰기 직전**에 하한 검사
     *        [ ] 4-2. out_arr[wordIndex] = i
     *        [ ] 4-3. wordIndex-- */

    /* [ ] 5. return 0 */
}
```
