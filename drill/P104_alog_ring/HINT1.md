# HINT1
구조체에 필요한 것: `char **q`(링 버퍼), `cap`, `head`, `count`, `full`, `stop`, 카운터 3개, `pthread_mutex_t`, `pthread_cond_t`, `pthread_t`.

- **쓰기**: 락 → `count == cap` 이면 `dropped++`, `full = 1`, 언락 후 `ALOG_FULL`. 아니면 `q[(head + count) % cap] = 복사본`, `count++`, `enq++`, `cond_signal`.
- **소비 스레드**: 락 → `count == 0 && !stop` 동안 `cond_wait` → 비었고 `stop` 이면 종료 → 한 줄 꺼내고 `head`, `count`, `deq` 갱신, FULL 해제 검사 → **언락** → sink 호출 → `free`.
- FULL 해제는 **꺼낼 때** 검사한다: `cap - count >= max(1, cap/10)`.
