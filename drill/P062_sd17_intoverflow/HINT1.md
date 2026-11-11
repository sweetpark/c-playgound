# HINT1
처방: 연산 전 오버플로우 검사로 치환

`malloc(n + reserve)` 나 `malloc(n * size)` 를 보면 오버플로우부터 의심한다.
