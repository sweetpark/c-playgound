# HINT2 (막혔을 때만)
```
if name==NULL or out==NULL or outsize==0: -1
v = getenv(name); if v == NULL: -1
n = strlen(v); if n+1 > outsize: -2
memcpy(out, v, n+1); 0
```
