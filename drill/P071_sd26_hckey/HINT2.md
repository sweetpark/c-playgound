# HINT2 (막혔을 때만)
```
const unsigned char *get_key_bad(void)
{
    /* 1. static const unsigned char HARD_KEY[16] = { 0x13, 0x2a, ... }
     * 2. return HARD_KEY   ← 항상 같은 키, 포인터 공유 */
}

int get_key_good(const char *key_name, unsigned char *out, size_t out_len)
{
    /* [ ] 1. 키는 이름으로 저장소에서 조회한다
     *        return mock_keystore_load(key_name, out, out_len)
     *        호출자 버퍼로 **복사**해서 준다 (포인터 공유 금지) */
}
```
