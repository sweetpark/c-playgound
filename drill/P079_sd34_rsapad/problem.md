# P079 · sd34 rsapad — 취약한 암호화 적절하지 못한 RSA 패딩 — OAEP 패딩 강제로 치환

## 요구사항

RSA 암호화에 사용할 패딩 모드를 검증한다.
- 패딩 없음(NO_PADDING)은 절대 허용하면 안 된다.
- OAEP, PKCS1 패딩은 허용한다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **OAEP 패딩 강제로 치환**.

## 시그니처

```c
int is_safe_padding_bad(padding_t p);
int is_safe_padding_good(padding_t p);
```

## 입출력 예시

| 패딩 모드 | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| `PAD_OAEP` (정상) | 허용 | 안전한 패딩 |
| `PAD_NONE` | **허용됨(취약)** | 교과서적 RSA는 결정적이라 같은 평문이 같은 암호문을 만들고, 선택 암호문 공격에 취약하다 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P079` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- Bad는 패딩 없는(NO_PADDING) RSA도 그대로 허용해야 한다(취약점 재현)
- Good은 패딩 없는 RSA를 거부해야 한다
- Good은 OAEP 패딩을 허용해야 한다
- Good은 PKCS1 패딩도 허용해야 한다
