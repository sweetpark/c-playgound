# HINT2 (막혔을 때만)
```
void prepare_for_send_bad(const char *pw, char *out, size_t out_size)
{
    /* 1. strncpy(out, pw, out_size - 1)
     * 2. out[out_size - 1] = '\0'   ← 원본 그대로 전송/저장 */
}

void prepare_for_send_good(const char *pw, char *out, size_t out_size)
{
    /* [ ] 1. xor_mask(in, out, out_size) 를 먼저 만든다
     *        i + 1 < out_size 이고 in[i] 가 있는 동안
     *            out[i] = in[i] ^ 0x5A
     *        out[i] = '\0' */

    /* [ ] 2. xor_mask(pw, out, out_size) 호출 */
}

void undo_mask(const char *masked, char *out, size_t out_size)
{
    /* [ ] 3. xor_mask(masked, out, out_size)   ← XOR 은 대칭이라 같은 함수로 복원 */
}
```
