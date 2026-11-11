# NOTE · P023 wait_signal (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] 환경변수·시그널 실습 — getenv 복사·atexit와 sigaction·pselect 시그널 카운터」 의 시그널 경쟁 구간 (pselect 시그널 카운터)
- 소스: [`c_basic/sig/sig_ex2.c`](../../c_basic/sig/sig_ex2.c), [`sig_ex2_v2.c`](../../c_basic/sig/sig_ex2_v2.c)

**함정 요약**
- `if (!flag) pause();` 는 검사와 대기 사이에 시그널이 오면 영원히 잠든다 → **막고** 기다리는 원자적 대기(`sigtimedwait`/`sigsuspend`/`pselect`).
- 마스크 복원을 빠뜨리면 호출자의 시그널이 영원히 막힌다. 원래 막혀 있었는지 여부도 보존해야 한다.
- `EINTR` 재시도.

**한 줄 설명 연습**: "시그널을 기다릴 때는 먼저 막아 두고 나서 기다려야 놓치지 않는다."
