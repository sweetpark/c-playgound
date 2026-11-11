# HINT1
가드: `CHK_PTR(s); CHK_RANGE(v, 0, 100);`. 자원 함수는 `ret`, `buf = NULL`, `fp = NULL` 로 시작하고 `CHK_PTR_GOTO` / `CHK_COND_GOTO(fp != NULL, RET_IO_ERROR, ret, CLEANUP)` 로 빠지며 끝에 `CLEANUP:` 한 곳.
