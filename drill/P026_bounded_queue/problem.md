# P026 · bounded_queue — mutex + 조건변수 2개 + close

## 요구사항
용량 8 의 정수 **블로킹 큐**를 mutex + 조건변수 2개(`not_empty`, `not_full`)로 만든다. 가득 차면 `push` 가, 비면 `pop` 이 기다린다. `close` 하면 기다리는 모든 스레드가 깨어나고, 남은 항목은 모두 꺼낼 수 있다.

## 시그니처
```c
void bq_init(bq_t *q);
void bq_destroy(bq_t *q);
int  bq_push(bq_t *q, int v);      /* 0 성공 / -1 닫힘 (가득 차면 기다림) */
int  bq_pop(bq_t *q, int *out);    /* 0 성공 / -1 닫혔고 비었음   (비면 기다림) */
void bq_close(bq_t *q);            /* 이후 push 는 -1. 대기 중인 스레드 전부 깨움 */
```

## 입출력 예시
| 상황 | 결과 |
|---|---|
| push 10, 20 → pop, pop | 10, 20 (FIFO) |
| push 1, 2 → close → push 3 | -1 |
| close 뒤 pop, pop, pop | 1, 2, -1 |
| 생산자 2 × 5000 개, 소비자 3, 마지막에 close | 소비 개수 10000, 합 정확 |

## 제약
- 조건 검사는 `if` 가 아니라 `while` (깨어난 뒤 다시 확인).
- `bq_close` 는 `pthread_cond_signal` 이 아니라 `broadcast` (대기자가 여럿).
- 폴링(`sleep` 루프)이나 센티널 값으로 종료를 알리지 않는다.
- 교착이 나면 60 초 뒤 `FAIL (timeout/deadlock)`.

## 통과 기준
`make P026` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
