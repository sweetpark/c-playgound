# HINT2 (막혔을 때만)
```
if path == NULL: -1
fp = fopen(path, "wx")
if fp == NULL: return errno == EEXIST ? 1 : -1
fclose(fp); return 0
```
