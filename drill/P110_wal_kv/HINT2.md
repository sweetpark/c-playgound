# HINT2
```
open(dir):
  stat(dir) 디렉터리 아님 → NULL
  load_snapshot: "LSN n" 읽고 "key val" 줄들 반영. "END" 못 보면 메모리 비우고 lsn=0
  good = 0
  loop fgets(line):
      끝이 '\n' 이고 sscanf("%lu %c %63s %63s %x%n") == 5 이고 crc 일치 아니면 break
      lsn <= cur  → good = ftell, continue          (건너뜀)
      lsn != cur+1 → break
      적용(P: put, D: del); cur = lsn; good = ftell
  truncate(wal.log, good)   ← 꼬리 제거
  wal = fopen(wal.log, "a")

crc: 줄에서 마지막 공백 앞까지 바이트 합 & 0xffff
```
