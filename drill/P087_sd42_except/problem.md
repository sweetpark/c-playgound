# P087 · sd42 except — 적절하지 않은 예외처리 — 반환값 검사 동반으로 치환

## 요구사항

표준입력에서 한 줄을 읽어 다른 버퍼로 복사한다.
- 읽기에 실패하면 복사를 시도해서는 안 된다.
- 읽기 함수의 반환값은 반드시 확인해야 한다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **반환값 검사 동반으로 치환**.

신뢰 경계:

| 값 | 출처 | 검증 없이 흘러가는 곳 |
| :--- | :--- | :--- |
| `fgets`(또는 이를 흉내낸 `mock_fgets`)의 반환값 | 표준입력 읽기 결과 | 반환값 확인 없이 바로 이어지는 `strcpy`의 원본 버퍼 |

## 시그니처

```c
void copy_input_bad(char *from_buf, size_t from_size, int simulate_read_fail, char *to_buf, size_t to_size);
int copy_input_good(char *from_buf, size_t from_size, int simulate_read_fail, char *to_buf, size_t to_size);
```

## 입출력 예시

| 상황 | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| 읽기 실패로 `fromBuf`가 널 종료 없이 쓰레기 값으로 채워짐 | **`strcpy`가 버퍼 경계를 넘어 읽어 크래시**(`expect_crash`가 감지) | 반환값을 검사하지 않아 실패 상태를 그대로 신뢰하고 다음 연산으로 넘어간다 |
| 같은 상황(Good) | **복사를 아예 하지 않고 `-1` 반환** | `retBuf != fromBuf` 로 실패를 확인한 즉시 중단한다 |
| 정상 읽기(둘 다) | 정상 복사 | 반환값이 예상대로일 때는 그대로 진행 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P087` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- Bad는 읽기 실패로 널 종료가 안 된 fromBuf를 그대로 strcpy해 오버플로우로 크래시해야 한다(취약점 재현)
- 정상 읽기에서는 크래시 없이 내용이 그대로 복사돼야 한다
- Good은 읽기 실패를 감지하면 복사를 하지 않고 실패를 반환해야 한다
- 정상 읽기는 성공해야 한다
- 정상 읽기에서는 내용이 정확히 복사돼야 한다
