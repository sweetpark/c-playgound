# P023 · wait_signal — 막고 기다리기 (sigtimedwait)

## 요구사항
시그널 `signo` 가 올 때까지 최대 `timeout_ms` 밀리초 기다린다. 시그널을 **먼저 막은 뒤**(`sigprocmask`) `sigtimedwait` 로 받아야 "검사 후 대기" 사이에 도착한 시그널을 놓치지 않는다. 호출 전의 시그널 마스크로 되돌린다.

## 시그니처
```c
int wait_signal(int signo, int timeout_ms);
/* 1  : 시그널을 받았다
 * 0  : 시간 초과
 * -1 : 오류 (signo <= 0, timeout_ms < 0, ...) */
```

## 입출력 예시
| 상황 | 반환 | 호출 후 마스크 |
|---|---|---|
| 100 ms 뒤 자식이 `SIGUSR2` 전송, 대기 3000 ms | 1 | 원래대로(막혀 있지 않음) |
| 아무도 안 보냄, 대기 200 ms | 0 | 원래대로 |
| 호출 전에 `SIGUSR2` 가 막힌 채 이미 도착 | 1 | 막힌 채로 복원 |
| `signo = -1`, `timeout_ms = -1` | -1 | |

## 제약
- 핸들러를 쓰지 않는다(`sigtimedwait` 만). 먼저 `sigprocmask(SIG_BLOCK)` 로 막고 이전 마스크를 저장.
- `EINTR`(다른 시그널에 끊김)와 `EAGAIN`(시간 초과)을 구분한다.
- 복원할 때 `errno` 가 덮어써지지 않게 한다(P014).

## 통과 기준
`make P023` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
