# HINT1
포인터 세 개(`r`, `nm`, `v`)를 모두 `NULL` 로 시작하고, 실패하면 `goto fail`. `fail:` 에서 **획득의 역순**으로 `free(v); free(nm); free(r);` — `free(NULL)` 은 안전하므로 "어디까지 성공했는지" 따질 필요가 없다.
`*out = r` 은 모든 할당이 성공한 맨 마지막에.
