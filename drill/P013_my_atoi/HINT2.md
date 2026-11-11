# HINT2 (막혔을 때만)
```
if s==NULL or out==NULL or s[0]==0: return -1
if isspace(s[0]): return -1
errno = 0
v = strtol(s, &end, 10)
if end == s or *end != 0: return -1
if errno == ERANGE or v > INT_MAX or v < INT_MIN: return -2
*out = (int)v; return 0
```
