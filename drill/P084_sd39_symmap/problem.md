# P084 · sd39 symmap — 심볼릭명이 정확한 대상에 매핑되어 있지 않음 — mkstemp로 치환

## 요구사항

요청마다 임시 작업 파일을 만들어 결과를 기록한다.
- 다른 프로세스가 이 파일의 실제 경로를 미리 알 수 없어야 한다.
- 파일명 충돌이 발생해서는 안 된다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **mkstemp로 치환**.

신뢰 경계:

| 값 | 출처 | 검증 없이 흘러가는 곳 |
| :--- | :--- | :--- |
| 임시 파일 경로(`guess.tmp`) | 코드에 고정된 이름 | 공격자가 서비스 시작 전에 미리 심볼릭 링크로 선점할 수 있는 좌표 |

## 시그니처

```c
int open_predictable_tmp_bad(const char *dir, const char *content);
int create_random_tmp_good(const char *dir, char *out_path, size_t out_size, const char *content);
```

## 입출력 예시

| 상황 | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| 공격자가 `guess.tmp` 자리에 `victim_target`을 가리키는 심볼릭 링크를 미리 심어둠 | **`victim_target`이 그대로 덮어써진다** | 이름이 고정이라 공격자가 서비스 실행 전에 정확한 좌표를 미리 알고 트랩을 심을 수 있다 |
| 같은 상황(Good) | **`victim_target`은 전혀 건드려지지 않는다** | `mkstemp()`가 만드는 실제 이름은 매번 다르고 예측 불가능해, 공격자가 미리 심어둔 `guess.tmp` 트랩과 절대 겹치지 않는다 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P084` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- mkdtemp
- HACKED
- HACKED
- Good 첫 번째 호출은 성공해야 한다
- Good 두 번째 호출도 성공해야 한다
- Good은 호출마다 다른 무작위 파일명을 만들어야 한다(예측 불가능해야 트랩을 피할 수 있다)
- Good이 만든 실제 경로는 공격자가 미리 심어둔 예측 가능한 이름과 달라야 한다
- ORIGINAL
- Good이 실제로 만든 파일에는 내용이 정확히 써져야 한다
