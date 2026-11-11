# P055 · sd10 proccontrol — 프로세스 제어 — 허용 명령 매핑으로 치환

## 요구사항

요청 파라미터(`op`)와 문서 이름(`documentName`)으로 변환 명령을 조립한다.
- 허용되는 변환은 `DocToHtml`, `DocToPdf` 두 가지뿐이다.
- `op` 값 자체가 그대로 실행 프로그램명이 되어서는 안 되고, 반드시 내부에 정의된 실제 명령어로 치환되어야 한다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **허용 명령 매핑으로 치환**.

신뢰 경계:

| 값 | 출처 | 검증 없이 흘러가는 곳 |
| :--- | :--- | :--- |
| `op` | 외부(요청 파라미터) | `system()` 에 전달될 명령어의 **프로그램 이름 부분** |

## 시그니처

```c
int build_convert_command_bad(const char *op, const char *documentName, char *out, size_t out_size);
int build_convert_command_good(const char *op, const char *documentName, char *out, size_t out_size);
```

## 입출력 예시

| 입력(`op`) | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| `"DocToHtml"` (정상) | `"DocToHtml report.doc"` | 정상 동작(단, `op` 값이 그대로 실행 프로그램명이 되므로 근본적으로 취약한 설계다) |
| `"rm -rf /tmp/x; echo pwned"` | **`"rm -rf /tmp/x; echo pwned report.doc"`** | `op` 자체가 임의의 명령으로 대체될 수 있다 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P055` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- Bad는 조립 자체는 항상 성공해야 한다
- Bad는 op 문자열이 그대로 명령어가 되어 임의 명령이 섞여 들어가야 한다(취약점 재현)
- DocToHtml report.doc
- Good은 매핑에 없는 op를 거부해야 한다
- 매핑에 있는 op는 통과해야 한다
- convertDocToHtml report.doc
