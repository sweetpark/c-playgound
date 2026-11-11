# P098 · sd53 sysdata — 시스템 데이터 정보노출 — 경로 제외 일반 메시지로 치환

## 요구사항

실행 파일을 찾지 못하거나 메모리 할당에 실패하면 오류 메시지를 남긴다.
- 오류 메시지에 `PATH` 환경변수나 메모리 주소 같은 시스템 내부 데이터가 포함되어서는 안 된다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **경로 제외 일반 메시지로 치환**.

신뢰 경계:

| 값 | 출처 | 검증 없이 흘러가는 곳 |
| :--- | :--- | :--- |
| `PATH` 환경변수 값 | `getenv("PATH")` | 화면(stderr)에 그대로 찍히는 오류 메시지 |
| 실패한 할당의 포인터 값 | `malloc()` 실패 시점의 인자 | 오류 메시지에 `%p`로 그대로 찍히는 지점 |

## 시그니처

```c
void report_exe_not_found_bad(const char *path_env, char *out, size_t out_size);
void report_exe_not_found_good(const char *path_env, char *out, size_t out_size);
void report_alloc_failure_bad(void *ptr, char *out, size_t out_size);
void report_alloc_failure_good(void *ptr, char *out, size_t out_size);
```

## 입출력 예시

| 상황 | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| 실행 파일 탐색 실패 | **`PATH` 값이 메시지에 그대로 노출** | `%s`로 경로 문자열을 그대로 끼워 넣는다 |
| 메모리 할당 실패 | **메모리 주소가 16진수로 노출**(`0x...`) | `%p`로 포인터 값을 그대로 찍는다 — 공격자가 ASLR을 우회하는 데 쓸 수 있는 메모리 배치 정보다 |
| 같은 상황(Good, 둘 다) | 경로·주소 정보 전혀 없이 일반 메시지만 | 값 자체를 메시지 포맷에서 뺀다 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P098` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- /home/user/.secret_bin
- /home/user/.secret_bin
- cannot find exe on path
- Bad는 메모리 주소를 16진수로 그대로 노출해야 한다(ASLR/메모리 배치 정보 유출 재현)
- Good은 오류 메시지에 메모리 주소를 절대 포함하면 안 된다
