# HINT1
`v = getenv(name)` 이 NULL 인지 보고, `n = strlen(v)`, `n + 1 > outsize` 면 -2, 아니면 `memcpy(out, v, n + 1)`.
