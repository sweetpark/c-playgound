# HINT2 (막혔을 때만)
```
ARRAY_LEN(a)       = (sizeof(a) / sizeof((a)[0]))
COPY_STR(dst, src) = snprintf(dst, sizeof(dst), "%s", src)     # 결과 (void) 로 버려도 됨
ZERO_ARRAY(a)      = memset(a, 0, sizeof(a))
```
매크로 안의 인자는 `(a)` 처럼 괄호로 감쌀 것.
