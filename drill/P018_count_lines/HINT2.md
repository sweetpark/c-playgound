# HINT2 (막혔을 때만)
```
fp = fopen(path, "r"); if !fp: -1
at_start = true
while fgets(buf, 16, fp):
    n = strlen(buf)
    if buf[n-1] == \n: lines++, at_start = true else at_start = false
err = ferror(fp); fclose(fp)
if err: return -2
if !at_start: lines++
return lines
```
