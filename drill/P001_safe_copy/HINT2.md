# HINT2 (막혔을 때만)
```
if dst == NULL or dstsize == 0 or src == NULL: return -1
n = strlen(src)
if n >= dstsize:            # 널 문자 자리까지 못 들어간다
    memcpy(dst, src, dstsize - 1)
    dst[dstsize - 1] = '\0'
    return 1
memcpy(dst, src, n + 1)     # 널 문자까지 복사
return 0
```
