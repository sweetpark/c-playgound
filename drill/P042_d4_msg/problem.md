# P042 · d4 — Pack·Unpack — 비트필드·엔디안·왕복시험

> 예상 시간: 30분. 이 드릴은 크므로 막히면 HINT 와 NOTE 를 적극적으로 쓰고, 며칠에 나눠 풀어도 된다.

## 요구사항

(변형 A — 자체 메시지 헤더)

> [!QUOTE] 규격서 (발췌) — 고정 헤더 12바이트 + payload + 체크섬 2바이트
> ```text
> 옥텟  7 6 5 4 3 2 1 0   (비트 번호, MSB=7)
> ──────────────────────────────────────────────────────────
>  0    │Ver(3) │ Type(5)         │   Ver 0~7, Type 0~31
>  1    │ Flags(8)                │   b0=URG b1=ACK b2=FRAG b3=ENC, b4~b7 예비
>  2-3  │ PayloadLen (16, BE)     │   0 ~ 1400
>  4-7  │ SessionId  (32, BE)     │
>  8-10 │ Sequence   (24, BE)     │   0 ~ 16,777,215
>  11   │Prio(4)│ Reserved(4)     │   Prio 0~15, Reserved 는 0 고정
> ──────────────────────────────────────────────────────────
>  12.. │ Payload (PayloadLen 바이트)
>  끝   │ Checksum (16, BE)       │   헤더+payload 전체의 인터넷 체크섬
> ```
> - 모든 다바이트 정수는 **네트워크 바이트 순서(BE)**.
> - `Reserved` 가 0이 아니면 **`RET_BAD_FORMAT`**.
> - `Flags` 의 **모르는 비트(b4~b7)는 지우지 말고 그대로 보존**한다.
>   (전방 호환)
> - 수신 시 체크섬 불일치는 **`RET_BAD_FORMAT`**.

### 미션

```c
int msg_pack  (const msg_t *in, uint8_t *buf, size_t buf_len, size_t *out_len);
int msg_unpack(const uint8_t *buf, size_t buf_len, msg_t *out, size_t *consumed);
void hexdump  (const char *tag, const uint8_t *buf, size_t len);
```

1. `rd_u16be` / `wr_u16be` / `rd_u24be` / `wr_u24be` / `rd_u32be` / `wr_u32be` 를 **직접** 구현 (`htons` 금지).
2. 인터넷 체크섬(1의 보수 합)을 직접 구현.
3. `NEED()` 공간검사 매크로를 만들고 **모든 쓰기/읽기 앞에** 건다.
4. **왕복 시험**: 임의 값 1000회 → pack → unpack → 필드 전부 일치.

**구조체 `msg_t` (A)**

| 필드 | 타입 | 범위 / 형식 | 비고 |
| :--- | :--- | :--- | :--- |
| `ver` | `uint8_t` | 0 ~ 7 | |
| `type` | `uint8_t` | 0 ~ 31 | |
| `flags` | `uint8_t` | 8비트 전부 | **모르는 비트(b4~b7)도 그대로 보존** |
| `prio` | `uint8_t` | 0 ~ 15 | |
| `session_id` | `uint32_t` | 전체 | |
| `seq` | `uint32_t` | 0 ~ 16,777,215 | 와이어에서는 24비트 |
| `payload_len` | `uint16_t` | 0 ~ 1400 | |
| `payload` | `uint8_t[1400]` | | |

**에러 코드 규칙 (A)**

| 상황 | 리턴코드 |
| :--- | :--- |
| 인자가 NULL | `RET_INVALID_ARG` |
| `pack`: `ver` > 7, `type` > 31, `prio` > 15 | `RET_INVALID_ARG` |
| `pack`: `payload_len` > 1400, `seq` > 0xFFFFFF | `RET_BAD_FORMAT` |
| `pack`: 출력 버퍼가 모자람 / `unpack`: 입력이 짧음 | `RET_TOO_SMALL` (**아무것도 쓰지 않는다**) |
| `unpack`: `Reserved` ≠ 0, `PayloadLen` > 1400, 체크섬 불일치 | `RET_BAD_FORMAT` |

**공통 규칙**

1. 실패하면 `*out_len` / `*consumed` 는 0, `out` 구조체는 전부 0 이다.
2. 체크섬은 **체크섬 자리를 제외한** 헤더 + payload 전체를 대상으로 계산한다.
3. 쓰기는 `NEED(필요한 전체 길이)` 를 **먼저** 하고, 그다음에 쓴다.

## 시그니처

```c
uint16_t inet_csum(const uint8_t *b, size_t len);
int msg_pack (const msg_t *in, uint8_t *buf, size_t buf_len, size_t *out_len);
int msg_unpack(const uint8_t *buf, size_t buf_len, msg_t *out, size_t *consumed);
```

(타입과 상수는 같은 폴더의 `*.h` 에 이미 있다. `solution.c` 에는 함수 본문만 채운다.)

## 입출력 예시

위 "에러" 표와 규칙의 예가 곧 입출력 예시다. 시험(`test.c`)이 대표 입력과 경계를 전부 확인한다.

## 제약

- 공통 헤더(`common.h`, `guard.h`, `log.h`, `safe.h`)의 가드·단일 출구·`goto CLEANUP` 패턴을 쓴다(`drill/common/`).
- 실패하면 출력 구조체는 0 (쓰레기를 남기지 않는다) 등 표의 규칙을 그대로 지킨다.
- 경고 0, ASan/UBSan/LSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P042` → `PASS`, 실패하면 `FAIL k` 와 어긋난 항목이 출력된다. 힌트: `HINT1.md` → `HINT2.md`.
