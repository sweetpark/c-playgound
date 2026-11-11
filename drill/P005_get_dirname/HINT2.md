# HINT2 (막혔을 때만)
```
if path==NULL or out==NULL or outsize==0: return -1
slash = strrchr(path, '/')
if slash == NULL: return -1
len = slash - path
if len == 0: len = 1                 # "/x" -> "/"
if len + 1 > outsize: return -1      # out 은 건드리기 전에 검사
memcpy(out, path, len); out[len] = 0
return 0
```
