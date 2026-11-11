# NOTE · P022 sig_counter (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] 환경변수·시그널 실습 — getenv 복사·atexit와 sigaction·pselect 시그널 카운터」 §2 "12-01 시그널 시점에 따라 일부가 무시됨", "12-02 signal() 은 시스템마다 동작이 다름", "12-03 핸들러 안에서 fputs/free", "12-04 핸들러가 strcpy 로 전역 버퍼를 건드림"
- 소스: [`c_basic/lecture/sig/`](../../c_basic/lecture/sig/), [`c_basic/sig/sig_ex.c`](../../c_basic/sig/sig_ex.c)

**함정 요약**
- `signal()` 은 구현마다 의미가 다르다(핸들러 유지 vs 1회 후 기본값). `sigaction` 으로 고정.
- 핸들러는 어느 코드 위에서든 끼어든다. `printf`/`malloc`/`free` 는 락을 잡고 있다가 교착. 그래서 핸들러는 플래그만, 처리는 main 에서.
- 공유 변수는 `volatile sig_atomic_t` — 컴파일러가 레지스터에 캐싱하지 못하게 하고, 한 번에 읽고 쓰기 보장.

**한 줄 설명 연습**: "핸들러는 최대한 짧게 플래그만 세우고, 실제 일은 핸들러 밖에서 한다."
