# HINT1
`if (step(ctx) != 0) { saved = errno; ... }` 를 먼저 하고, 그 뒤에 `cleanup(ctx)`. 마지막에 `errno = saved` 로 되돌려 놓고 `saved` 를 반환.
