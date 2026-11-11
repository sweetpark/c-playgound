# HINT1
`copy = malloc(strlen(s)+1)` 후 `strtok_r(copy, delims, &save)` 루프. 오류 경로가 여러 개이므로 `goto done` 한 곳에서 `free(copy)`.
