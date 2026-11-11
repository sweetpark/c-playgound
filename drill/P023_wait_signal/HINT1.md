# HINT1
`sigemptyset/sigaddset` 으로 집합 → `sigprocmask(SIG_BLOCK, &set, &old)` → `sigtimedwait(&set, NULL, &ts)` (반환 >=0 받음, `errno==EAGAIN` 시간 초과) → `sigprocmask(SIG_SETMASK, &old, NULL)`.
