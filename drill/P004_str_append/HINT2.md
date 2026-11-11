# HINT2 (막혔을 때만)
```
if dst==NULL or src==NULL or dstsize==0: return -1
dlen = strnlen(dst, dstsize)
if dlen == dstsize: return -1                # 널이 범위 안에 없음
slen = strlen(src)
if slen > dstsize - dlen - 1: return -1      # dlen < dstsize 이므로 뺄셈 안전
memcpy(dst + dlen, src, slen + 1)            # 널 문자까지
return 0
```
