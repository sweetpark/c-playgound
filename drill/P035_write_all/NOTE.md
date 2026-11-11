# NOTE · P035 write_all (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] 에러 처리·API 설계 실습 — strtol my_atoi·snprintf_m·sum_file·conformant 배열」 §3 "14-04 non-blocking write 가 EAGAIN 일 때 대기 없이 재시도 → select/poll 로 대기"
- 소스: [`c_basic/lecture/err/`](../../c_basic/lecture/err/) (14-xx 계열)

**함정 요약**
- `write(fd, buf, n)` 이 `n` 보다 적게 돌려줘도 오류가 아니다. 남은 부분을 이어서 써야 데이터가 안 잘린다.
- `EAGAIN` 에 바로 재시도하면 상대가 읽을 때까지 CPU 를 태운다. `poll`/`select` 로 쓰기 가능 이벤트를 기다린다.
- 읽는 쪽이 닫혀 있으면 `SIGPIPE`(기본 동작: 프로세스 종료) 또는 `EPIPE` — 시그널은 호출 쪽에서 무시하거나 `MSG_NOSIGNAL`.

**한 줄 설명 연습**: "write 는 한 번에 다 쓴다는 보장이 없어서, 쓴 만큼 건너뛰며 EINTR 은 재시도, EAGAIN 은 poll 로 기다린다."
