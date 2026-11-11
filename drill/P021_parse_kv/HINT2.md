# HINT2 (막혔을 때만)
```
if args bad: -1
key[0] = val[0] = 0
eq = strchr(line, '='); if !eq: -2
trim [line,eq) -> kb,ke; if empty: -3; if len+1 > key_size: -4
trim [eq+1,end) -> vb,ve; if len+1 > val_size: -5
memcpy key, val (+ NUL); 0
```
