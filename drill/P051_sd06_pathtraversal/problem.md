# P051 · sd06 pathtraversal — 디렉터리 경로 조작 — 절대경로 정규화 검증으로 치환

## 요구사항

`sandbox_root` 디렉터리 안의 파일만 조회를 허용한다.
- 파일명(`rName`)은 사용자가 지정한다.
- `sandbox_root` 밖의 파일에는 어떤 경우에도 접근할 수 없어야 한다.
- 존재하지 않는 파일 요청은 실패로 처리한다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **절대경로 정규화 검증으로 치환**.

신뢰 경계:

| 값 | 출처 | 검증 없이 흘러가는 곳 |
| :--- | :--- | :--- |
| `rName` | 외부(요청 파라미터) | 파일 열기 경로 |

## 시그니처

```c
char *resolve_report_path_bad(const char *sandbox_root, const char *rName, char *out, size_t out_size);
int resolve_report_path_good(const char *sandbox_root, const char *rName, char *out, size_t out_size);
```

## 입출력 예시

| 입력(`rName`) | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| `report1.txt` (정상) | sandbox 안의 파일 내용 | 정상 동작 |
| `../secret.txt` | **sandbox 밖 `secret.txt` 의 실제 내용을 읽어낸다** | 문자열을 그대로 이어붙여 상위 디렉터리로 빠져나간다 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P051` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- mkdtemp
- TOP-SECRET
- public report
- Good은 sandbox 밖 경로를 거부해야 한다
- public report
