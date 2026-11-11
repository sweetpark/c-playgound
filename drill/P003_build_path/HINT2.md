# HINT2 (막혔을 때만)
```
if out==NULL or outsize==0 or dir==NULL or file==NULL or file[0]==0: return -1
dlen = strlen(dir)
sep  = (dlen == 0 or dir[dlen-1] == '/') ? "" : "/"
n = snprintf(out, outsize, "%s%s%s", dir, sep, file)
if n < 0 or n >= outsize: out[0] = 0; return -1
return 0
```
