# P077 · sd32 pwplain — 패스워드 평문 저장 — 해시 저장으로 치환

## 요구사항

설정 파일에 저장된 값과 사용자가 입력한 패스워드를 대조해 인증한다.
- 저장된 값 자체에서 원본 패스워드를 알 수 없어야 한다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **해시 저장으로 치환**.

## 시그니처

```c
unsigned toy_hash(const char *s);
void save_password_bad(const char *pw, char *stored, size_t size);
int verify_password_bad(const char *stored, const char *input);
void save_password_good(const char *pw, unsigned *stored);
int verify_password_good(unsigned stored, const char *input);
```

## 입출력 예시

| 저장 방식 | 저장된 값에서 원본 패스워드를 알 수 있는가 | 위험 |
| :--- | :--- | :--- |
| 평문(Bad) | **그대로 보인다** | 설정 파일 하나만 유출되면 패스워드 전체 노출 |
| 해시(Good) | 알 수 없다(단방향) | 유출돼도 원본 복원 불가 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P077` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- hunter2
- hunter2
- 틀린 패스워드는 실패해야 한다
- 저장된 해시값이 존재해야 한다(타입 자체가 평문일 수 없음을 구조로 보증)
- hunter2
- 틀린 패스워드는 실패해야 한다
