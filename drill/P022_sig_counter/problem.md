# P022 · sig_counter — sigaction 과 핸들러에서 할 수 있는 일

## 요구사항
시그널 `signo` 를 받을 때마다 올라가는 카운터를 만든다. `signal()` 대신 `sigaction` 으로 설치하고, 핸들러는 **카운터만** 올린다(`printf`·`free`·`strcpy` 금지). 핸들러와 공유하는 변수의 타입은 `volatile sig_atomic_t`.

## 시그니처
```c
int  sigc_install(int signo);     /* 0 성공 / -1 실패 (SIGKILL, 잘못된 번호 ...) */
int  sigc_uninstall(int signo);   /* 기본 동작(SIG_DFL)으로 복원. 0 / -1 */
int  sigc_count(void);            /* 지금까지 받은 횟수 */
void sigc_reset(void);            /* 카운터 0 */
```

## 입출력 예시
| 동작 | 결과 |
|---|---|
| 설치 후 `raise(SIGUSR1)` 3 번 | `sigc_count() == 3` |
| `sigc_reset()` | 0 |
| `sigc_uninstall(SIGUSR1)` | `sigaction` 으로 조회하면 `SIG_DFL` |
| `sigc_install(SIGKILL)`, `(-5)` | -1 |

## 제약
- `sa_flags` 에 `SA_RESETHAND`(1회용)·`SA_NODEFER`(처리 중 같은 시그널 허용) 를 주지 않는다. `SA_SIGINFO` 없는 단순 핸들러.
- 핸들러 안에서는 async-signal-safe 하지 않은 함수를 부르지 않는다.
- 카운터를 읽는 쪽도 같은 타입으로.

## 통과 기준
`make P022` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
