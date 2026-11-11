# HINT2 (막혔을 때만)
```
put: for i in 0..7: out[i] = (v >> (56 - 8*i)) & 0xFF
get: v = 0; for i in 0..7: v = (v << 8) | in[i]; return v
```
