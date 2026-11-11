# HINT1
`push`: `lock → while (count == CAP && !closed) wait(not_full) → closed 면 unlock/-1 → 저장, count++ → signal(not_empty) → unlock`. `pop` 은 대칭. `close` 는 `closed = 1` 후 두 조건변수 모두 `broadcast`.
