# HINT1
`#define FREE(p) do { free(p); (p) = NULL; } while (0)`, `#define XNEW(p) ((p) = alloc_zeroed_array(1, sizeof(*(p))))`. 도우미는 `n == 0 || n > SIZE_MAX / sz` 이면 NULL, 아니면 `calloc(n, sz)`.
