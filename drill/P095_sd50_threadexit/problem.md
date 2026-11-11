# P095 · sd50 threadexit — 코드정확성 스레드 조기 종료 — join 또는 detach 명시로 치환

## 요구사항

작업을 처리할 워커 스레드를 생성한다.
- 생성한 스레드는 반드시 join 또는 detach로 회수 여부가 결정돼야 한다.
- 결과값은 필요하면 안전하게 받아올 수 있어야 한다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **join 또는 detach 명시로 치환**.

신뢰 경계:

| 값 | 출처 | 검증 없이 흘러가는 곳 |
| :--- | :--- | :--- |
| (해당 없음) | — | join/detach 결정 누락 자체가 결함이다 |

## 시그니처

```c
int unreaped_count(void);
void reset_unreaped(void);
int spawn_worker_bad(int *result);
int spawn_worker_good(int *result);
```

## 입출력 예시

| 상황 | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| `spawn_worker_bad()` 호출 | **미회수 스레드 카운트가 `1`로 남음** | `pthread_join`도 `pthread_detach`도 호출하지 않아 스레드 자원의 운명이 결정되지 않은 채 함수가 끝난다 |
| 같은 상황(Good) | 미회수 카운트가 `0`으로 복귀, 결과값도 확실히 반영됨 | `pthread_join`으로 완료를 기다린 뒤 회수한다 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P095` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- 스레드 생성 자체는 성공해야 한다
- Bad는 join도 detach도 하지 않아 생성한 스레드가 회수되지 않은 채로 남아야 한다(취약점 재현)
- 스레드 생성과 join이 모두 성공해야 한다
- Good은 join으로 즉시 회수해 미회수 카운트가 0이어야 한다
- join으로 완료를 기다렸으므로 워커의 결과가 반영돼 있어야 한다
