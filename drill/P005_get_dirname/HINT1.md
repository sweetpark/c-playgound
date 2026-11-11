# HINT1
`const char *slash = strrchr(path, '/');` 로 위치만 찾고, 길이는 `len = slash - path`.
`out` 에 `memcpy(out, path, len)` 후 `out[len] = '\0'`. 검사는 `len + 1 > outsize` (널 문자 자리). `len == 0` 이면 루트라서 `"/"`.
