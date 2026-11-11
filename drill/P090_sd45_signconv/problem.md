# P090 · sd45 signconv — 부호 정수를 무부호 정수로 타입 변환 오류 — 안전한 에러값으로 치환

## 요구사항

문자열의 길이를 구해 인덱스 접근 범위를 검사하는 데 사용한다.
- 문자열이 `NULL`이면 그 어떤 인덱스도 "범위 안"으로 판정되어서는 안 된다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **안전한 에러값으로 치환**.

신뢰 경계:

| 값 | 출처 | 검증 없이 흘러가는 곳 |
| :--- | :--- | :--- |
| `len_bad()`의 반환값(`unsigned int`) | `NULL` 에러 표시(`-1`) | 이후 `idx < len` 같은 경계 검사식의 우변 |

## 시그니처

```c
unsigned int len_bad(const char *s);
unsigned int len_good(const char *s);
int within_len_bad(unsigned int idx, const char *s);
int within_len_good(unsigned int idx, const char *s);
```

## 입출력 예시

| 입력 | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| `len_bad(NULL)` | `4294967295`(`UINT_MAX`) | `-1`이 `unsigned int`로 변환되며 최댓값으로 감긴다 |
| `within_len_bad(999999, NULL)` | **`참`(범위 안으로 착각)** | `999999 < 4294967295`이 참이 되어, 원래는 무조건 거부돼야 할 접근이 통과해버린다 |
| 같은 상황(Good) | `within_len_good(999999, NULL)` → **`거짓`** | `len_good(NULL)`이 `0`이라 `999999 < 0`은 항상 거짓(무부호라 음수 자체가 없음에도 검사가 의도대로 작동) |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P090` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- Bad는 NULL에서 -1을 unsigned로 반환해 UINT_MAX로 감겨야 한다(취약점 재현)
- 정상 문자열은 길이를 정확히 반환해야 한다
- Good은 NULL에서 무부호에 안전한 0을 반환해야 한다
- 정상 문자열은 길이를 정확히 반환해야 한다
- Bad는 -1이 UINT_MAX로 감겨서 어떤 큰 idx도 범위 안으로 착각해야 한다(경계검사 무력화 재현)
- Good은 0을 반환하므로 같은 idx가 범위 밖으로 올바르게 거부돼야 한다
