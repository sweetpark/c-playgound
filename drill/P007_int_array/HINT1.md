# HINT1
`calloc(n, sizeof(int))` 은 0 으로 채워 주지만, `n` 이 외부 값이면 호출 **전에** 범위·곱셈 한계를 직접 검사하는 습관을 들인다 (`n > SIZE_MAX / sizeof(int)`).
해제 함수는 이중 포인터로 받아 `free(*pp); *pp = NULL;`.
