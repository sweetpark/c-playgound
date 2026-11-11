# P035 · write_all — 부분 쓰기·EINTR·EAGAIN

## 요구사항
`fd` 에 `n` 바이트를 **전부** 쓴다. `write` 는 요청보다 적게 쓸 수 있고(부분 쓰기), 시그널에 끊기거나(`EINTR`), 비차단 fd 에서는 `EAGAIN` 이 날 수 있다. 바쁜 재시도 없이 `poll` 로 쓸 수 있을 때까지 기다린다.

## 시그니처
```c
int write_all(int fd, const void *buf, size_t n);
/* 0  : n 바이트 모두 씀 (n == 0 이면 바로 0)
 * -1 : fd < 0, buf == NULL(n > 0), EPIPE 등 쓰기 오류, 10 초 동안 쓸 수 없음 */
```

## 입출력 예시
| 상황 | 결과 |
|---|---|
| 비차단 파이프에 30 만 바이트, 느린 리더 | 0, 리더가 받은 바이트 수·합·순서 일치 |
| `n = 0` | 0 |
| `fd = -1` / `buf = NULL, n = 5` | -1 |
| 읽는 쪽이 닫힌 파이프 (`EPIPE`) | -1 |

## 제약
- `write` 의 반환값(쓴 바이트)만큼 포인터를 전진하고 남은 길이를 줄인다. 반환값을 `n` 과 비교만 하고 버리면 안 된다.
- `EINTR` 는 재시도. `EAGAIN`/`EWOULDBLOCK` 은 즉시 재시도(CPU 100%)하지 말고 `poll(POLLOUT)`.
- 그 외 오류는 즉시 -1.

## 통과 기준
`make P035` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
