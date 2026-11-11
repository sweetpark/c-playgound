# P073 · sd28 cipherchoice — 취약한 암호화 알고리즘 사용 — 식별자 화이트리스트 검증으로 치환

## 요구사항

암호화에 사용할 알고리즘 식별자 문자열을 검사해 허용 여부를 판정한다.
- `AES-*`, `ARIA-*`, `SEED-*` 계열만 허용한다.
- `DES`, `3DES`, `RC4`, `MD5`, `SHA1` 은 명시적으로 금지한다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **식별자 화이트리스트 검증으로 치환**.

## 시그니처

```c
int is_allowed_cipher_bad(const char *cipher_name);
int is_allowed_cipher_good(const char *cipher_name);
```

## 입출력 예시

| 알고리즘 식별자 | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| `AES-256-GCM` (정상) | 허용 | 정상 동작 |
| `DES-ECB` | **허용됨(취약)** | 검사 자체가 없다 |
| `MD5` | **허용됨(취약)** | 해시 무결성용 알고리즘도 그냥 통과 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P073` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- DES-ECB
- Bad는 MD5도 허용해야 한다
- DES-ECB
- Good은 RC4를 거부해야 한다
- Good은 SHA1을 거부해야 한다
- AES-256-GCM
- SOME-UNKNOWN-CIPHER
