# HINT1
`int n = snprintf(out, outsize, ...)` 의 `n` 은 "잘리지 않았다면 쓰였을 길이"다.
`n < 0 || (size_t)n >= outsize` 이면 실패. 구분자 `/` 는 `dir` 의 마지막 글자를 보고 `""` 또는 `"/"` 로 정한다(`dir` 가 빈 문자열일 때 `dir[strlen(dir)-1]` 주의).
