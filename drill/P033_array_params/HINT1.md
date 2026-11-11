# HINT1
누적은 `long s`. 최댓값은 `m = a[0]` 로 시작해 `i = 1` 부터. 복사는 `dn < sn` 이면 `-1`, 아니면 `memcpy(dst, src, sn * sizeof(src[0]))`.
