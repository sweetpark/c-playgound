# P046 · sd01 sqli — SQL 삽입 — 인자화된 질의문으로 치환

## 요구사항

`username`, `password` 두 문자열을 받아 회원 인증을 수행한다.
- 인증 성공 시 매칭된 회원 인덱스(0 이상)를, 실패 시 `-1` 을 반환한다.
- `username` 은 최대 8자, `password` 는 최대 16자.
- 회원 데이터는 `mock_db` 안에 고정되어 있다(직접 건드리지 않는다).

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **인자화된 질의문으로 치환**.

신뢰 경계:

| 값 | 출처 | 검증 없이 흘러가는 곳 |
| :--- | :--- | :--- |
| `userId` | 외부(로그인 폼) | `where` 절 문자열 조립 |
| `password` | 외부(로그인 폼) | `where` 절 문자열 조립 |

## 시그니처

```c
int login_bad(const char *userId, const char *password);
int login_good(const char *userId, const char *password);
```

## 입출력 예시

| 입력(`password`) | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| `pw123!` (정상) | 로그인 성공(인덱스 0) | 정상 동작 |
| `x' OR '1'='1` | **로그인 성공(인덱스 0)** | `where` 문자열이 `...password='x' OR '1'='1'` 가 되어 뒤 조건이 항상 참 |
| `wrong` | 로그인 실패(-1) | 정상 동작(오답 처리) |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P046` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- x' OR '1'='1
- pw123!
- 오답 처리 실패
- x' OR '1'='1
- pw123!
- hunter2
