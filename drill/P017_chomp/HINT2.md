# HINT2 (막혔을 때만)
```
if s == NULL: return 0
n = strlen(s)
if n > 0 and s[n-1] == \n:
    n--
    if n > 0 and s[n-1] == \r: n--
    s[n] = 0
return n
```
