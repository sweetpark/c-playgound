# P025 · transfer — 락 두 개, 획득 순서 고정

## 요구사항
두 계좌 사이 이체 `transfer(from, to, amt)` 를 만든다. 서로 **반대 방향** 이체가 동시에 일어나도 교착하지 않아야 한다. 잔액 확인과 차감은 같은 락 구간 안에서 한다.

## 시그니처
```c
typedef struct { int id; pthread_mutex_t mu; long bal; } acct_t;   /* solution.c 에 이미 있음 */

int transfer(acct_t *from, acct_t *to, long amt);
/* 0  : 이체됨
 * -1 : NULL, 같은 계좌(from == to), amt <= 0, 잔액 부족  (잔액 변화 없음) */
```

## 입출력 예시
| 상황 | 결과 |
|---|---|
| A(100만)→B(100만) 300 | 0, A = 999700, B = 1000300 |
| 잔액보다 큰 금액 / 0 / 음수 | -1, 변화 없음 |
| `transfer(&a, &a, 10)` | -1 (같은 락을 두 번 잡지 않는다) |
| 스레드 4 개: A→B, B→A, A→B, B→A 각 10만 번 | 교착 없이 끝, 총합 200만 유지 |

## 제약
- 두 락을 항상 **같은 순서**(id 오름차순)로 잡는다. 인자 순서(from, to)대로 잡으면 A→B 와 B→A 가 서로 기다린다.
- 푸는 순서는 획득의 역순. 모든 경로(성공/실패)에서 푼다.
- 교착이 나면 테스트가 60 초 뒤 `FAIL (timeout/deadlock)` 로 끝낸다.

## 통과 기준
`make P025` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
