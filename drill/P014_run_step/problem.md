# P014 · run_step — 정리 코드가 errno 를 덮어쓰기 전에 저장

## 요구사항
`step` 함수를 한 번 실행하고, 성공/실패와 관계없이 `cleanup` 을 **한 번** 호출한다.
`step` 이 0 이 아닌 값을 돌려주면 실패다. 실패한 **직후의** `errno` 를 저장해 두었다가, `cleanup` 이 `errno` 를 바꿔도 **원래 값**을 반환하고 `errno` 에도 복원한다.

## 시그니처
```c
int run_step(int (*step)(void *), void (*cleanup)(void *), void *ctx);
/* 0           : step 성공
 * errno 값(>0): step 실패 시 그 직후의 errno.  단 errno == 0 이었다면 EIO
 * EINVAL      : step == NULL (이때 cleanup 은 부르지 않는다)
 * cleanup == NULL 이면 정리 없이 진행.  실패 시 반환 직전 errno = 반환값 */
```

## 입출력 예시
| step | cleanup | 반환 | 반환 후 `errno` |
|---|---|---|---|
| 성공 | `errno = EBADF` 로 덮어씀 | 0 | (규정 없음) |
| `errno = ENOENT` 후 -1 | `errno = EBADF` 로 덮어씀 | `ENOENT` | `ENOENT` |
| `errno = EACCES` 후 1 | 〃 | `EACCES` | `EACCES` |
| `errno = 0` 인데 -1 | 〃 | `EIO` | `EIO` |
| `NULL` | 〃 | `EINVAL` | cleanup 호출 0 회 |

## 제약
- `errno` 저장은 `step` 이 돌아온 **바로 다음 문장**에서. 그 사이에 `printf`·`free`·`close` 같은 호출을 넣지 않는다.
- `cleanup` 은 정확히 한 번(성공·실패 모두).

## 통과 기준
`make P014` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
