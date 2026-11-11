# HINT2 (막혔을 때만)
```
sum:  if !a: 0; s = 0 (long); for i in 0..n: s += a[i]
max:  if n==0 or !a or !out: -1; m = a[0]; for i in 1..n: m = max(m, a[i]); *out = m; 0
copy: if !dst or !src: -1; if dn < sn: -1; if sn: memcpy(dst, src, sn*sizeof *src); 0
```
