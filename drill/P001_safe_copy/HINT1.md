# HINT1
`strncpy(dst, src, n)` 은 `src` 가 `n` 글자 이상이면 널 문자를 **붙이지 않는다**.
복사량은 `dstsize - 1` 까지만, 마지막 칸은 직접 `'\0'`. (또는 `snprintf(dst, dstsize, "%s", src)` 의 반환값으로 잘림 판정)
