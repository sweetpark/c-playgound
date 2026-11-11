# P065 · sd20 idor — 부적절한 인가 — 세션 식별자 대조로 치환

## 요구사항

`GET /record?user=<targetUsername>` 형태로 특정 사용자의 레코드를 조회한다.
- 요청자는 세션ID(`sid`)를 함께 보낸다.
- **자기 자신의 레코드만** 조회할 수 있어야 한다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **세션 식별자 대조로 치환**.

신뢰 경계:

| 값 | 출처 | 검증 없이 흘러가는 곳 |
| :--- | :--- | :--- |
| `targetUsername` | 외부(요청 파라미터) | 조회 허용 여부 판정 |

## 시그니처

```c
int view_record_bad(const char *sid, const char *targetUsername);
int view_record_good(const char *sid, const char *targetUsername);
```

## 입출력 예시

| 상황 | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| alice 세션으로 alice 레코드 조회 | 허용 | 정상 |
| **alice 세션으로 bob 레코드 조회** | **허용됨(IDOR)** | 세션 유효성만 확인하고 대상과 대조하지 않는다 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P065` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- Bad는 alice로 로그인해도 bob의 레코드를 조회할 수 있어야 한다(IDOR 재현)
- 로그인 안 된 세션은 거부되어야 한다
- Good은 alice 세션으로 bob 레코드를 조회하면 거부해야 한다
- Good은 본인 레코드는 조회를 허용해야 한다
