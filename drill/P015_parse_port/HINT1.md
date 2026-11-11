# HINT1
`strtoul` 은 `"-1"` 을 `ULONG_MAX` 로 래핑해 돌려주고 에러도 내지 않는다. 그래서 변환 **전에** 첫 글자가 숫자인지(`isdigit`) 확인한다.
그다음 `errno = 0` → `strtoul` → `end == s || *end` → `ERANGE` → `v < 1 || v > 65535` 순서.
