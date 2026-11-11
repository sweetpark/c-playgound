# P068 · sd23 privdrop — 최소 권한 적용 위배 — 즉시 권한 복귀로 치환

## 요구사항

특정 작업은 상위 권한(root)으로 수행해야 한다.
- 작업이 끝나면 반드시 원래 권한으로 돌아와야 한다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **즉시 권한 복귀로 치환**.

## 시그니처

```c
int get_current_priv(void);
void reset_priv(void);
void elevate_bad(void);
void do_privileged_work_good(void);
```

## 입출력 예시

| 상황 | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| `elevate_bad()` 호출 후 | **권한이 계속 root(0)로 남아있다** | 반납 코드가 아예 없다 |
| `do_privileged_work_good()` 호출 후 | 원래 권한(1000)으로 복귀 | 작업 직후 즉시 반납 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P068` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- Bad는 권한 상승 후 계속 root(0) 상태로 남아있어야 한다(취약점 재현)
- Good은 작업이 끝나면 상승 전 권한으로 정확히 복귀해야 한다
