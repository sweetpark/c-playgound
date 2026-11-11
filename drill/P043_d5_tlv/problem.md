# P043 · d5 — TLV 파서 — 모르는 태그 건너뛰기

> 예상 시간: 30분. 이 드릴은 크므로 막히면 HINT 와 NOTE 를 적극적으로 쓰고, 며칠에 나눠 풀어도 된다.

## 요구사항

(변형 A — 세션 메시지 body)

> [!QUOTE] 규격서 (발췌)
> 메시지 body 는 **TLV 목록**이다.
> 순서는 보장되지 않는다.
>
> ```text
> T(1) │ L(1) │ V(L bytes)
> L = 0xFF 이면 확장 길이: 뒤이어 2바이트(BE)가 실제 길이
> ```
>
> | IEI | 이름 | P | 길이 | 값 |
> | :--- | :--- | :--- | :--- | :--- |
> | `0x01` | SessionId | **M** | 4 | uint32 BE |
> | `0x02` | Cause | **M** | 1 | uint8 (0~63) |
> | `0x10` | UeName | O | 0~32 | UTF-8, 널 없음 |
> | `0x20` | Timestamp | O | 8 | uint64 BE (epoch ms) |
> | `0x30` | Nested | O | 가변 | **내부가 또 TLV** (깊이 1까지만) |
> | 그 외 | (미지원) | — | 가변 | **건너뛰되 원문 보존** |
>
> - 필수 IE 가 없으면 `RET_BAD_FORMAT`.
> - 같은 IEI 가 두 번 오면 **첫 번째를 채택**하고 경고 로그.
> - 모르는 IEI 는 **최대 8개까지 원문(T·L·V 전체)을 보관**해, 응답 생성 시 **그대로 되돌려 보낸다.**
> - IE 가 버퍼 경계를 넘어가면 `RET_BAD_FORMAT`.

### 미션

```c
int  tlv_parse (const uint8_t *buf, size_t len, msg_body_t *out);
int  tlv_build (const msg_body_t *in, uint8_t *buf, size_t buf_len, size_t *out_len);
```

1. 순회 골격을 **`while` 한 개**로.
   IE 마다 `case` 로 분기.
2. 모르는 IEI 는 `out->unknown[]` 에 **T·L·V 통째로** 보관 (8개 초과분은 버리고 WRN).
3. `tlv_build` 는 **알려진 IE → 모르는 IE 원문** 순으로 다시 쌓는다.
4. **왕복**: `parse(build(x)) == x` (모르는 IE 포함).

**구조체 `msg_body_t` (A)**

| 필드 | 타입 | 비고 |
| :--- | :--- | :--- |
| `session_id` / `has_session_id` | `uint32_t` / `bool` | M |
| `cause` / `has_cause` | `uint8_t` / `bool` | M |
| `name` / `has_name` | `char[33]` / `bool` | O, **널 종단은 내가 붙인다** |
| `ts_ms` / `has_ts` | `uint64_t` / `bool` | O |
| `unknown[8]` / `unknown_num` | `{ uint8_t iei; uint16_t len; uint8_t val[128]; }` | 모르는 IE 보관 (128 바이트 초과 또는 9번째부터는 **버리고 WRN**) |

**에러 코드 규칙 (A)**

| 상황 | 리턴코드 | 로그 |
| :--- | :--- | :--- |
| `buf` / `out` 이 NULL | `RET_INVALID_ARG` | 가드가 남긴다 |
| IE 가 버퍼 경계를 넘음 (T·L 자리 부족, 확장 길이 자리 부족, V 가 L 보다 짧음) | `RET_BAD_FORMAT` | O |
| IE 의 길이가 규격과 다름 (`SessionId` L≠4, `Cause` L≠1, `UeName` L>32, `Timestamp` L≠8) | `RET_BAD_FORMAT` | O |
| `Cause` 가 63 초과 | `RET_BAD_FORMAT` | O |
| `Nested` 안에 `Nested` (깊이 2) | `RET_BAD_FORMAT` | O |
| 필수 IE(`SessionId`, `Cause`) 누락 | `RET_BAD_FORMAT` | O (**순회가 끝난 뒤**) |
| 모르는 IEI | **`RET_OK`** (보관, 못 하면 WRN) | WRN |
| 중복 IEI | **`RET_OK`**, 첫 번째 채택 | WRN |

**함수와 규칙**

```c
int tlv_parse(const uint8_t *buf, size_t len, msg_body_t *out);
int tlv_build(const msg_body_t *in, uint8_t *buf, size_t buf_len, size_t *out_len);
```

1. `tlv_parse` 는 순회를 **`while` 한 개**로, IE 마다 `switch` 로 분기한다.
2. `tlv_build` 는 **`SessionId` → `Cause` → `UeName`(있으면) → `Timestamp`(있으면) → 모르는 IE 원문** 순으로 쌓는다.
   버퍼가 모자라면 `RET_TOO_SMALL` 이고 `*out_len` 은 0 이다.
3. `Nested`(0x30) 의 V 는 또 TLV 목록이다 (깊이 1까지만).
   내부 IE 도 같은 규칙으로 파싱해서 **같은 `out` 에 합친다.**
4. **왕복**: `parse(build(x))` 는 `x` 와 같다 (모르는 IE 포함).
5. 입력 `buf` 는 NULL 종단이 아니고, 길이 필드를 믿을 수 없다.

## 시그니처

```c
int tlv_parse(const uint8_t *buf, size_t buf_len, msg_body_t *out);
int tlv_build(const msg_body_t *in, uint8_t *buf, size_t buf_len, size_t *out_len);
```

(타입과 상수는 같은 폴더의 `*.h` 에 이미 있다. `solution.c` 에는 함수 본문만 채운다.)

## 입출력 예시

위 "에러" 표와 규칙의 예가 곧 입출력 예시다. 시험(`test.c`)이 대표 입력과 경계를 전부 확인한다.

## 제약

- 공통 헤더(`common.h`, `guard.h`, `log.h`, `safe.h`)의 가드·단일 출구·`goto CLEANUP` 패턴을 쓴다(저장소 `common/`).
- 실패하면 출력 구조체는 0 (쓰레기를 남기지 않는다) 등 표의 규칙을 그대로 지킨다.
- 경고 0, ASan/UBSan/LSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P043` → `PASS`, 실패하면 `FAIL k` 와 어긋난 항목이 출력된다. 힌트: `HINT1.md` → `HINT2.md`.
