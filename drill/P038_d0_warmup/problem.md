# P038 · d0 워밍업 — 가드와 CLEANUP 뼈대

## 요구사항
템플릿 체화 드릴 D0(워밍업). 저장소 `common/` 의 공통 뼈대(`common.h` 리턴코드, `guard.h` 가드 매크로, `log.h`, `safe.h` 정리 매크로)를 **백지에서 손으로** 써 본다. 함수 둘:
1. `take_ptr(s, v)` — 인자 가드 두 개. `s` 가 NULL 이면, `v` 가 0~100 을 벗어나면 `RET_INVALID_ARG`. 통과하면 로그를 남기고 `RET_OK`.
2. `take_res(path)` — 자원을 잡는 함수. `buf`(힙 64바이트)를 잡고 `path` 를 `fopen("r")`. 중간에 실패하면 **한 곳(`CLEANUP:`)** 에서 `fclose` / `free` 하고 나간다. `fopen` 실패는 `RET_IO_ERROR`. 성공하면 안에서 `take_ptr("inner", 10)` 을 부르고 그 결과를 따른다.

## 시그니처
```c
static int take_ptr(const char *s, int v);
static int take_res(const char *path);
/* 리턴: common.h 의 RET_OK / RET_INVALID_ARG / RET_IO_ERROR ... */
```

## 입출력 예시
| 호출 | 결과 |
|---|---|
| `take_ptr(NULL, 10)` | `RET_INVALID_ARG` |
| `take_ptr("hi", 999)`, `("hi", -1)`, `("hi", 101)` | `RET_INVALID_ARG` |
| `take_ptr("hi", 0)`, `("hi", 100)` | `RET_OK` (경계) |
| `take_res(NULL)` | `RET_INVALID_ARG` |
| `take_res(<있는 파일>)` | `RET_OK` |
| `take_res("/no/such/file")` | `RET_IO_ERROR` (힙은 해제됨) |

## 제약
- 자원을 잡기 **전**의 가드는 즉시 `return`(`CHK_PTR`, `CHK_RANGE`), 잡은 **뒤**의 실패는 `*_GOTO` 계열로 `CLEANUP:` 으로 간다.
- `CLEANUP:` 에서는 `SAFE_FCLOSE(fp)`, `SAFE_FREE(buf)` (획득의 역순, `NULL` 로 시작해서 어디서 실패해도 안전).
- 성공·실패 경로를 200 번씩 돌려도 파일 디스크립터와 힙이 새지 않아야 한다(시험이 확인).

## 통과 기준
`make P038` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
