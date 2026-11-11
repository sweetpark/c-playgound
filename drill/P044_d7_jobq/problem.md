# P044 · d7 — 데몬 — 시그널·poll·워커큐·graceful shutdown

> 예상 시간: 30분 (큐 단위). 이 드릴은 크므로 막히면 HINT 와 NOTE 를 적극적으로 쓰고, 며칠에 나눠 풀어도 된다.

## 요구사항

(변형 A — UDP 메시지 데몬)

> [!QUOTE] 요구사항서 (발췌)
> D4 의 메시지 포맷을 받아 처리하는 상주 프로세스를 만든다.
> - 기동 시 설정 파일(D3 의 `cfg_t`)에서 `listen_port`, `worker_num`(1~64), `log_level` 을 읽는다.
> - UDP 소켓 하나를 열고 `poll()` 로 수신을 기다린다.
>   **바쁜 대기(busy loop) 금지.**
> - 수신 메시지를 `msg_unpack()` 으로 파싱해 **워커 큐**에 넣는다.
>   파싱 실패는 **폐기 + WRN**, 프로세스는 계속 산다.
> - 워커 N개가 큐에서 꺼내 처리하고 응답을 송신한다.
> - `SIGINT` / `SIGTERM`: **graceful shutdown** — 새 수신을 멈추고, **큐에 남은 것까지 처리**한 뒤 종료.
>   단 **최대 5초**.
> - `SIGHUP`: 설정 **재적재**(로그 레벨만 반영, 포트 변경은 무시).
> - 종료 코드: 정상 0, 설정 오류 1, 소켓 오류 2, 스레드 오류 3.
> - 어떤 경로로 종료하든 **누수 0**.

### 미션

1. `main()` 을 **계단식 라벨**로 작성 (초기화 역순 해제).
2. `sigaction` 으로 `SIGINT`/`SIGTERM`/`SIGHUP` 등록 + `SIGPIPE` 무시.
3. 스레드 안전 큐 (`pthread_mutex_t` + `pthread_cond_t`), **종료 시 `broadcast`**.
4. `poll()` 타임아웃 200ms — 소켓에 아무것도 안 와도 **종료 깃발을 확인**할 수 있게.
5. 종료 대기에 **상한 5초** (`clock_gettime` + `pthread_cond_timedwait`).

**설정 파일 (D3 의 `cfg_t`)**

| 키 | 범위 | 비고 |
| :--- | :--- | :--- |
| `listen_port` | 1 ~ 65535 | |
| `worker_num` | 1 ~ 64 | |
| `log_level` | `ERR` / `WRN` / `INF` / `DBG` | `SIGHUP` 으로 **재적재** 대상 |

**로그 계약 — 시험이 이 문자열을 `grep` 한다**

| 시점 | 레벨 | 형식 |
| :--- | :--- | :--- |
| 기동 완료 | INF | `started: port=<n> worker=<n>` |
| 파싱 성공 후 **큐에 넣었을 때** | INF | `recv session=<id>` |
| 워커가 **처리를 끝냈을 때** | INF | `done session=<id>` |
| 파싱 실패로 폐기 | WRN | `drop: <사유>` |
| 설정 재적재 | INF | `config reloaded` |
| 종료 시작 | INF | `shutting down...` |

**종료 코드**

| 코드 | 의미 |
| :--- | :--- |
| 0 | 정상 종료 |
| 1 | 설정 오류 (파일 없음 포함) — **소켓·스레드를 만들기 전에** 종료 |
| 2 | 소켓 오류 (포트 사용 중 등) — **스레드를 만들기 전에** 종료 |
| 3 | 스레드·큐 오류 |

**작업 큐 `jobq_t` (D7 의 핵심 부품)**

| 함수 | 요구사항 |
| :--- | :--- |
| `int jobq_create(size_t cap, jobq_t **out)` | `cap == 0` 또는 `out` NULL → `RET_INVALID_ARG`.<br>**실패하면 `*out = NULL`.** |
| `int jobq_push(jobq_t *q, const job_t *job)` | 닫힌 큐 → `RET_NOT_SUPPORTED`, **가득 참 → `RET_TOO_SMALL`** (버린다, 막지 않는다) |
| `int jobq_pop(jobq_t *q, job_t *out)` | 비어 있으면 **대기**한다 (`cond_wait` + `while`).<br>**닫혔고 비었으면 `RET_NOT_FOUND`.**<br>닫혔어도 **남은 것은 꺼낼 수 있다** (drain). |
| `void jobq_close(jobq_t *q)` | 더 이상 받지 않는다.<br>**대기 중인 스레드를 전부 깨운다** (`broadcast`).<br>NULL 안전. |
| `void jobq_destroy(jobq_t *q)` | NULL 안전. |

`job_t` 는 `uint32_t session_id`, `uint8_t type`, `uint16_t len`, `uint8_t data[1400]` 와 송신자 주소(`struct sockaddr_in from`)를 가진다.

**추가 규칙 (A)**

1. `main()` 은 초기화 단계마다 **계단식 라벨**을 하나씩 둔다 (`ERR_THREAD` → `ERR_QUEUE` → `ERR_SOCK` → `ERR_CFG`).
2. 스레드 인자는 **힙에 복사**해서 넘기고 스레드가 해제한다.
3. `pthread_create` 가 실패하면 **만든 것(`made`)만** 되돌린다.
4. 종료 대기에는 **상한 5초**를 둔다.
5. 파싱 실패는 **폐기 + WRN**, 프로세스는 계속 산다.

## 시그니처

```c
int jobq_create(size_t cap, jobq_t **out);
int jobq_push(jobq_t *q, const job_t *job);
int jobq_pop(jobq_t *q, job_t *out);
void jobq_close(jobq_t *q);
void jobq_destroy(jobq_t *q);
```

(타입과 상수는 같은 폴더의 `*.h` 에 이미 있다. `solution.c` 에는 함수 본문만 채운다.)

## 입출력 예시

위 "에러" 표와 규칙의 예가 곧 입출력 예시다. 시험(`test.c`)이 대표 입력과 경계를 전부 확인한다.

## 제약

- 공통 헤더(`common.h`, `guard.h`, `log.h`, `safe.h`)의 가드·단일 출구·`goto CLEANUP` 패턴을 쓴다(`drill/common/`).
- 실패하면 출력 구조체는 0 (쓰레기를 남기지 않는다) 등 표의 규칙을 그대로 지킨다.
- 경고 0, ASan/UBSan/LSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P044` → `PASS`, 실패하면 `FAIL k` 와 어긋난 항목이 출력된다. 힌트: `HINT1.md` → `HINT2.md`.
