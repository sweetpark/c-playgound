# HINT1
- 구조체: `dir`, 키/값 배열(`keys`, `vals`, `n`, `cap`), `lsn`, `FILE *wal`(이어쓰기 `"a"`).
- 한 줄 쓰기: `body = "<lsn> P <key> <val>"`, 줄 = `body + " " + "%04x"(바이트 합 % 65536) + "\n"`. 쓴 뒤 `fflush` → `fsync(fileno(wal))`.
- 재생 루프: `fgets` 로 한 줄씩. 줄이 `\n` 으로 끝나는지, `sscanf` 가 5개를 읽는지, crc 가 맞는지, `lsn` 이 `<=`/`==+1`/그 밖인지 순서대로 검사. **유효한 줄의 끝 오프셋(`ftell`)**을 기억해 둔다.
- 재생이 끝나면 `truncate(path, 유효한_끝)` 으로 꼬리를 자르고, 그다음에 `"a"` 로 연다.
- 체크포인트: `snapshot.tmp` → `fflush`/`fsync` → `rename` → `fopen(wal,"w")` 로 비우기 → 다시 `"a"`.
