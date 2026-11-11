# HINT2 (막혔을 때만)
```
void gen_token_bad(unsigned seed, char *out, size_t out_size)
{
    /* 1. srand(seed)   ← seed 만 알면 재현된다
     * 2. out_size - 1 번: out[i] = "0123456789abcdef"[rand() % 16]
     * 3. out[i] = '\0' */
}

int gen_token_good(char *out, size_t out_size)
{
    /* [ ] 1. 지역변수
     *        unsigned char raw[16], size_t i */

    /* [ ] 2. 버퍼 크기 검사
     *        out_size < sizeof(raw) * 2 + 1 → return -1 */

    /* [ ] 3. 커널 CSPRNG
     *        getrandom(raw, sizeof(raw), 0) != (ssize_t)sizeof(raw) → return -1
     *        실패하면 약한 값으로 대체하지 않고 **중단** */

    /* [ ] 4. 16진수로 변환
     *        sprintf(out + i * 2, "%02x", raw[i])  (i = 0 ~ 15) */

    /* [ ] 5. return 0 */
}
```
