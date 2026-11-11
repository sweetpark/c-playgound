# HINT2 (막혔을 때만)
```
blob_new(n):   if n==0 or n>1024: NULL
               b = malloc(sizeof *b + n*sizeof b->data[0]); b->n = n; memset(b->data, 0, n*sizeof ...)
blob_clone(s): if !s: NULL
               bytes = sizeof *s + s->n * sizeof s->data[0]
               d = malloc(bytes); memcpy(d, s, bytes); return d
blob_sum(b):   for i in 0..b->n: sum += b->data[i]
```
