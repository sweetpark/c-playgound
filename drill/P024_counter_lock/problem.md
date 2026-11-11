# P024 · counter_lock — 읽기까지 같은 락, ERRORCHECK 뮤텍스

## 요구사항
여러 스레드가 함께 갱신하는 카운터를 뮤텍스로 보호한다. 값을 바꾸는 `counter_add` 뿐 아니라 **읽는 `counter_get` 도 같은 락**을 잡는다. 뮤텍스는 개발 중 실수를 알려 주는 `PTHREAD_MUTEX_ERRORCHECK` 로 만든다.

## 시그니처
```c
typedef struct { pthread_mutex_t mu; long v; } counter_t;   /* solution.c 에 이미 있음 */

int  counter_init(counter_t *c);            /* ERRORCHECK 뮤텍스, v = 0.  0 / -1 */
void counter_add(counter_t *c, long d);     /* v += d */
long counter_get(counter_t *c);
int  counter_destroy(counter_t *c);         /* pthread_mutex_destroy 결과 (0 성공) */
```

## 입출력 예시
| 상황 | 결과 |
|---|---|
| 초기화 직후 | `counter_get == 0` |
| 잡지 않은 뮤텍스를 `pthread_mutex_unlock` | `EPERM` (ERRORCHECK 라서) |
| 스레드 4 개가 각 20만 번 `+1`, 다른 스레드가 `+3`/`-3` 반복 | 합계 정확히 80만 |
| 모두 join 한 뒤 `counter_destroy` | 0 |

## 제약
- 변수에 접근하는 **모든 경로**(읽기 포함)가 같은 락을 잡는다.
- `return` 뒤에 `unlock` 을 쓰지 않는다 — 값을 지역 변수에 담고 `unlock` 한 뒤 `return`.
- 잡은 함수가 푼다(한 번 lock 에 한 번 unlock).

## 통과 기준
`make P024` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
