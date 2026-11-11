# P088 · sd43 nullderef — 널 포인터 역참조 — 반환값 검사 후 사용으로 치환

## 요구사항

설정 키에 해당하는 값을 복사해 반환한다.
- 존재하지 않는 키가 들어와도 프로세스가 죽어서는 안 된다.
- 실패는 호출자에게 알려야 한다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **반환값 검사 후 사용으로 치환**.

신뢰 경계:

| 값 | 출처 | 검증 없이 흘러가는 곳 |
| :--- | :--- | :--- |
| `lookup_env_like()`의 반환값 | 설정 조회 결과(`getenv`류) | 검사 없이 바로 넘어가는 `strncpy`의 원본 포인터 |

## 시그니처

```c
const char *lookup_env_like(const char *key);
void copy_env_bad(const char *key, char *out, size_t out_size);
int copy_env_good(const char *key, char *out, size_t out_size);
```

## 입출력 예시

| 입력(`key`) | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| `"MISSING_VAR"`(존재하지 않는 키) | **NULL 역참조로 크래시**(`expect_crash`가 감지) | `lookup_env_like`가 `NULL`을 반환해도 검사 없이 바로 `strncpy`에 넘긴다 |
| `"HOME"`(존재하는 키, Bad에서도) | 정상 복사 | 정상 입력에서는 Bad도 문제없이 동작한다 — "항상 죽는다"가 아니라 "특정 입력에서만 죽는다"는 걸 확인 |
| `"MISSING_VAR"`(Good) | `-1` 반환, 크래시 없음 | 역참조 전에 `NULL` 검사로 걸러낸다 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P088` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- Bad는 알 수 없는 키에서 NULL을 검사 없이 역참조해 크래시해야 한다(취약점 재현)
- /home/appuser
- Good은 NULL을 검사해 알 수 없는 키에서 실패를 반환해야 한다
- 알려진 키에서는 성공해야 한다
- /home/appuser
