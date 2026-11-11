# HINT2 (막혔을 때만)
```
if s==NULL or out==NULL or !isdigit((unsigned char)s[0]): return -1
errno = 0
v = strtoul(s, &end, 10)
if end == s or *end != 0: return -1
if errno == ERANGE or v < 1 or v > 65535: return -1
*out = (unsigned short)v; return 0
```
