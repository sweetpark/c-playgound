# P041 · d3 — 자원 잡는 함수 — goto CLEANUP·create·destroy

> 예상 시간: 30분. 이 드릴은 크므로 막히면 HINT 와 NOTE 를 적극적으로 쓰고, 며칠에 나눠 풀어도 된다.

## 요구사항

(변형 A — 설정 파일 로더)

> [!QUOTE] 요구사항서 (발췌)
> `key=value` 형식 설정 파일을 읽어 설정 객체를 만든다.
> - 한 줄 최대 **255자**.
>   `#` 로 시작하는 줄과 빈 줄은 **무시**.
> - `key` 는 최대 31자, `value` 는 최대 127자.
>   앞뒤 공백은 **제거**.
> - 같은 key 가 두 번 나오면 **나중 것이 이긴다**.
> - 항목 개수 상한은 **없다** (동적 확장).
> - **필수 키**: `listen_port`(1–65535), `worker_num`(1–64).
>   없으면 로딩 실패.
> - 파일이 없으면 `RET_IO_ERROR`, 형식 위반은 `RET_BAD_FORMAT`, 할당 실패는 `RET_NO_MEM`.
> - 조회 API: `cfg_get_str(cfg, key, &out)` / `cfg_get_int(cfg, key, &out)`.

### 미션

1. `cfg_create(const char *path, cfg_t **out_cfg)` — **소유권을 넘기는 생성 함수**.
2. `cfg_destroy(cfg_t *cfg)` — `NULL` 안전.
3. 항목 배열을 **2배씩 확장**(`realloc`)하되, **realloc 실패 시 원본을 잃지 않게**.
4. `cfg_get_int()` 는 숫자가 아니면 `RET_BAD_FORMAT`.

**함수**

| 함수 | 요구사항 |
| :--- | :--- |
| `int cfg_create(const char *path, cfg_t **out_cfg)` | 파일을 읽어 객체를 만든다.<br>**실패하면 `*out_cfg` 는 NULL** 이다. |
| `void cfg_destroy(cfg_t *cfg)` | NULL 을 받아도 안전하다. |
| `int cfg_get_str(const cfg_t *cfg, const char *key, const char **out)` | 없는 키는 `RET_NOT_FOUND` |
| `int cfg_get_int(const cfg_t *cfg, const char *key, int *out)` | 없는 키는 `RET_NOT_FOUND`, 숫자가 아니면 `RET_BAD_FORMAT` |

**에러 코드 규칙 (A)**

| 상황 | 리턴코드 | 로그 |
| :--- | :--- | :--- |
| `path` / `out_cfg` 가 NULL | `RET_INVALID_ARG` | 가드가 남긴다 |
| 파일을 열 수 없음 | `RET_IO_ERROR` | O (`LOG_ERRNO`) |
| `=` 없는 줄, 빈 key, key 32자 이상, value 128자 이상, 필수 키 범위 위반 | `RET_BAD_FORMAT` | O |
| 필수 키(`listen_port`, `worker_num`)가 없음 | `RET_NOT_FOUND` | O |
| 할당 실패 (객체, 줄 버퍼, 항목 배열) | `RET_NO_MEM` | O |

## 시그니처

```c
int cfg_create(const char *path, cfg_t **out_cfg);
void cfg_destroy(cfg_t *cfg);
int cfg_get_str(const cfg_t *cfg, const char *key, const char **out);
int cfg_get_int(const cfg_t *cfg, const char *key, int *out);
```

(타입과 상수는 같은 폴더의 `*.h` 에 이미 있다. `solution.c` 에는 함수 본문만 채운다.)

## 입출력 예시

위 "에러" 표와 규칙의 예가 곧 입출력 예시다. 시험(`test.c`)이 대표 입력과 경계를 전부 확인한다.

## 제약

- 공통 헤더(`common.h`, `guard.h`, `log.h`, `safe.h`)의 가드·단일 출구·`goto CLEANUP` 패턴을 쓴다(`drill/common/`).
- 실패하면 출력 구조체는 0 (쓰레기를 남기지 않는다) 등 표의 규칙을 그대로 지킨다.
- 경고 0, ASan/UBSan/LSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P041` → `PASS`, 실패하면 `FAIL k` 와 어긋난 항목이 출력된다. 힌트: `HINT1.md` → `HINT2.md`.
