# NOTE · P024 counter_lock (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] 동시성·스레드 실습 — mutex 락 범위·데드락·조건변수와 메시지 큐·이체 감시 스레드」 §1 "15-01 credit() 에만 락이 없음", "15-05 return 뒤에 unlock, 읽기/토글에 락 없음", "15-07 NORMAL 뮤텍스는 잘못된 unlock 을 알려 주지 않음", "15-08 destroy 시점"
- 소스: [`c_basic/lecture/conc/15-01.c`](../../c_basic/lecture/conc/15-01.c), [`15-05.c`](../../c_basic/lecture/conc/15-05.c), [`15-07.c`](../../c_basic/lecture/conc/15-07.c), [`c_basic/conc/con_ex.c`](../../c_basic/conc/con_ex.c)

**함정 요약**
- 같은 변수를 두 함수가 접근하는데 한쪽만 락을 잡으면 락이 없는 것과 같다. 읽기도 포함.
- `return` 뒤의 `unlock` 은 실행되지 않는다 → 락이 영원히 잡힌 채.
- NORMAL 뮤텍스는 잘못된 unlock 을 조용히 허용한다. ERRORCHECK 는 `EPERM`, 이중 lock 은 `EDEADLK` 로 개발 중에 잡는다.
- `destroy` 는 모든 스레드가 `join` 된 뒤에.

**한 줄 설명 연습**: "공유 변수에 닿는 모든 경로가 같은 락을 잡고, 잡은 함수가 그 자리에서 푼다."
