# HINT2 (막혔을 때만)
```
alloc_zeroed_array(n, sz): if n==0 or sz==0 or n > SIZE_MAX/sz: NULL; return calloc(n, sz)
FREE(p):  do { free(p); (p) = NULL; } while (0)
XNEW(p):  ((p) = alloc_zeroed_array(1, sizeof(*(p))))
XNEWN(p,n): ((p) = alloc_zeroed_array((n), sizeof(*(p))))
```
