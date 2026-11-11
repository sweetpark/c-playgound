# HINT2 (막혔을 때만)
```
r = nm = v = NULL
if out==NULL or name==NULL or nvals==0 or nvals>1024: return -1
r  = malloc(sizeof *r);            if !r:  goto fail
nm = malloc(strlen(name) + 1);     if !nm: goto fail
v  = malloc(nvals * sizeof *v);    if !v:  goto fail
memcpy(nm, name, strlen(name)+1); memset(v, 0, nvals*sizeof *v)
r->name = nm; r->vals = v; r->nvals = nvals
*out = r; return 0
fail:
    free(v); free(nm); free(r); return -1

rec_destroy(r): if r: free(r->vals); free(r->name); free(r)
```
