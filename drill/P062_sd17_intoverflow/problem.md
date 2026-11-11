# P062 · sd17 intoverflow — 정수 오버플로우 — 연산 전 오버플로우 검사로 치환

## 요구사항

`count` 개의 int를 저장할 버퍼에 여유 공간(`RESERVE=8`)을 더해 할당한다.
- `count` 가 매우 커서 오버플로우가 나면 할당을 거부해야 한다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **연산 전 오버플로우 검사로 치환**.

## 시그니처

```c
void *int_alloc_bad(int count);
void *int_alloc_good(int count);
void use_buffer_bad(int count);
void *use_buffer_good(int count);
```

## 입출력 예시

| `count` | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| 10 (정상) | 정상 할당 | 오버플로우 없음 |
| `INT_MAX - 4` | **UBSan이 오버플로우를 리포트 → 결과값이 음수 → size_t 캐스팅으로 거대해짐 → ASan이 할당 요청을 거부하며 abort** | `count + RESERVE` 가 `INT_MAX` 를 넘는다 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P062` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- Bad는 오버플로우로 할당이 실패해도 반환값을 안 보고 써서 크래시해야 한다(취약점 재현)
- 정상 범위의 count는 크래시 없이 동작해야 한다
- Good은 덧셈 전에 오버플로우를 검사해 NULL을 반환해야 한다
- 정상 범위는 성공해야 한다
