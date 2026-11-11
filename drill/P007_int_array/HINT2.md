# HINT2 (막혔을 때만)
```
int_array_new(n):
    if n == 0 or n > MAX(1<<20): return NULL
    if n > SIZE_MAX / sizeof(int): return NULL
    return calloc(n, sizeof(int))

int_array_free(pp):
    if pp == NULL: return
    free(*pp)        # free(NULL) 은 안전
    *pp = NULL
```
