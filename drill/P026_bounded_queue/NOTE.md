# NOTE · P026 bounded_queue (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] 동시성·스레드 실습 — mutex 락 범위·데드락·조건변수와 메시지 큐·이체 감시 스레드」 §4 "15-12 cond_signal 은 임의의 대기자 하나만 깨움", "15-00 큐에 동기화 없음, 폴링, 센티널 → mutex + 조건변수 2개 + close_q"
- 소스: [`c_basic/lecture/conc/15-12.c`](../../c_basic/lecture/conc/15-12.c), [`c_basic/conc/msgq/`](../../c_basic/conc/msgq/)

**함정 요약**
- `if (count == 0) wait` — 거짓 깨움·다른 소비자가 먼저 꺼낸 경우에 빈 큐에서 읽는다. 반드시 `while`.
- `close` 에 `signal` 만 쓰면 대기자 하나만 깨어나고 나머지는 영원히 잔다(300 회 중 1 회 정지 수준의 드문 버그).
- 대기 조건에 `!closed` 가 없으면 close 해도 못 깨어난다.
- 닫힌 뒤에도 남은 항목은 비워 줘야 데이터 손실이 없다.

**한 줄 설명 연습**: "조건변수는 while 로 조건을 다시 확인하고, 종료 신호는 broadcast 로 모두 깨운다."
