# P063 · sd18 signext — 의도하지 않은 부호 확장 — 원본 타입으로 직접 검사하도록 치환

## 요구사항

사용자 ID(`id`)로 300칸짜리 정보 배열을 조회한다.
- `id` 는 0 이상 300 미만이어야 한다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **원본 타입으로 직접 검사하도록 치환**.

## 시그니처

```c
int lookup_info_bad(int id, const int *info);
int lookup_info_good(int id, const int *info, size_t info_size);
```

## 입출력 예시

| `id` | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| 10 (정상) | 정상 값 반환 | 유효 범위 |
| **40000** | **SIGSEGV로 크래시** | `short` 로 잘리며 `-25536` 이 되어 `s > 256` 검사를 통과, 이후 `unsigned` 로 뒤집혀 `info[4294941760]` 접근 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P063` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- Bad는 short로 잘리며 음수가 된 값이 거대한 unsigned 인덱스가 되어 크래시해야 한다(취약점 재현)
- 정상 범위의 id는 크래시 없이 동작해야 한다
- Good은 범위를 넘는 id를 원본 int로 직접 검사해 거부해야 한다
- 음수 id도 거부해야 한다
- 정상 id는 값을 정확히 반환해야 한다
