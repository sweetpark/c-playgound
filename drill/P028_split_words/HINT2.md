# HINT2 (막혔을 때만)
```
if bad args: -1
copy = malloc + memcpy(s)
for tok = strtok_r(copy, delims, &save); tok; tok = strtok_r(NULL, delims, &save):
    if strlen(tok)+1 > 16: rc=-2; goto done
    if n == maxn: rc=-3; goto done
    memcpy(out[n++], tok, len+1)
rc = n
done: free(copy); return rc
```
