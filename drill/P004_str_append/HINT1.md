# HINT1
길이를 `dlen = strlen(dst)`, `slen = strlen(src)` 로 **루프 전에 한 번** 구해 변수에 둔다.
검사식은 `dlen + slen + 1 > dstsize` (널 문자 자리 +1). 단, `dst` 에 널이 없으면 `strlen(dst)` 자체가 위험하니 `memchr(dst, '\0', dstsize)` 나 `strnlen` 으로 먼저 확인.
