# HINT2 (막혔을 때만)
```
if !fp or !out: -1
while (r = fscanf(fp, "%ld", &v)) == 1: if __builtin_add_overflow(sum, v, &sum): return -4
if r == 0: return -3                     # 숫자가 아닌 토큰
if ferror(fp) or !feof(fp): return -2    # EOF 인데 정상 끝이 아님 = 읽기 오류
*out = sum; return 0
```
