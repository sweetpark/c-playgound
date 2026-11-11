# HINT2 (막혔을 때만)
```
validate all args first (-1)
out[0] = 0; pos = 0
for i in 0..cnt:
    n = snprintf(out+pos, outsize-pos, "%s%s", i>0 ? sep : "", items[i])
    if n < 0 or n >= outsize-pos: out[0]=0; return -2
    pos += n
return 0
```
