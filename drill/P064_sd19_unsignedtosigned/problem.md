# P064 · sd19 unsignedtosigned — 무부호 정수를 부호 정수로 타입 변환 오류 — signed로 받아 검증 후 변환으로 치환

## 요구사항

"초기화됨" 상태일 때만 지정된 크기만큼 `src` 를 `dst` 로 복사한다.
- 초기화가 안 됐으면 크기 조회 함수는 에러(`-1`)를 반환한다.
- 에러 상태에서는 복사를 수행하면 안 된다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **signed로 받아 검증 후 변환으로 치환**.

## 시그니처

```c
int get_chunk_size(int initialized, int chunk_size);
void chunk_copy_bad(void *dst, const void *src, int initialized, int chunk_size);
int chunk_copy_good(void *dst, size_t dst_size, const void *src, int initialized, int chunk_size);
```

## 입출력 예시

| `initialized` | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| 1 (정상, 정상 크기) | 정상 복사 | 에러 없음 |
| **0 (미초기화)** | **memcpy가 약 4GB 복사를 시도하며 크래시** | `get_chunk_size()` 가 반환한 `-1` 을 `unsigned` 로 받아 `4294967295` 가 된다 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P064` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- Bad는 초기화 실패(-1)가 거대한 unsigned 크기로 바뀌어 memcpy가 폭주해야 한다(취약점 재현)
- 정상 초기화 상태는 크래시 없이 동작해야 한다
- Good은 초기화 실패(-1)를 부호 있는 채로 확인해 거부해야 한다
- 정상 초기화 상태는 성공해야 한다
- 복사된 내용이 정확해야 한다
