# HINT2 (막혔을 때만)
```
int is_safe_padding_bad(padding_t p)
{
    /* 1. (void)p
     * 2. return 1   ← PAD_NONE 도 허용 */
}

int is_safe_padding_good(padding_t p)
{
    /* [ ] 1. return p != PAD_NONE
     *        위험한 값 **하나**만 거른다 */
}
```
