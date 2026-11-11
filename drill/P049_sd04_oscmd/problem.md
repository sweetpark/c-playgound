# P049 · sd04 oscmd — OS 명령어 삽입 — 위험 문자 검사로 치환

## 요구사항

사용자가 지정한 파일명(`userArg`)으로 `cat <파일명>` 명령을 조립해 실행한다.
- 조립된 명령이 실제로 몇 개의 명령으로 나뉘어 실행될지(`mock_shell_command_count`)를 반환한다.
- 정상 파일명은 항상 명령 1개로 끝나야 한다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **위험 문자 검사로 치환**.

신뢰 경계:

| 값 | 출처 | 검증 없이 흘러가는 곳 |
| :--- | :--- | :--- |
| `userArg` | 외부(표준입력) | `system()` 에 전달될 명령 문자열 |

## 시그니처

```c
int build_and_count_bad(const char *userArg);
int build_and_count_good(const char *userArg);
```

## 입출력 예시

| 입력 | Bad 결과(명령 개수) | 이유 |
| :--- | :--- | :--- |
| `Story.txt` (정상) | 1 | 정상 동작 |
| `Story.txt; ls` | **2** | 세미콜론 뒤 `ls` 가 별도 명령으로 실행된다 |
| `Story.txt \ | rm -rf /` | (파이프, mock에서는 명령 1개로 세지만 실제 셸에서는 위험) | Good에서 파이프도 함께 차단 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P049` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- Story.txt; ls
- Story.txt
- Story.txt; ls
- Story.txt | rm -rf /
- Story.txt
