# HINT2 (막혔을 때만)
```
if fp==NULL or buf==NULL or size==0: return -2
len = 0; nread = 0; truncated = 0
while (c = fgetc(fp)) != EOF and c != '\n':
    nread++
    if len + 1 < size: buf[len++] = c
    else: truncated = 1
buf[len] = 0
if c == EOF and nread == 0: return -1
return truncated ? 1 : 0
```
