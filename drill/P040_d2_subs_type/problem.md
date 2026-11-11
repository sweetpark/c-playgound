# P040 · d2 — 표 3장 → 구조체 — Optional·Conditional·union·정렬

> 예상 시간: 30분 (큰 편: 이틀로 나눠도 됨). 이 드릴은 크므로 막히면 HINT 와 NOTE 를 적극적으로 쓰고, 며칠에 나눠 풀어도 된다.

## 요구사항

(변형 A — 가입자 프로파일 레코드)

> [!QUOTE] 요구사항서 (발췌)
> 가입자 프로파일 조회 응답을 담을 자료구조를 설계한다.
>
> | # | 필드 | 타입 | P | 값 범위 / 형식 | 설명 |
> | :--- | :--- | :--- | :--- | :--- | :--- |
> | 1 | `supi` | string | **M** | 숫자 15자 이하 | 가입자 식별자 |
> | 2 | `nickname` | string | O | UTF-8 31바이트 이하 | 표시 이름 |
> | 3 | `ratType` | enum | O | `NR` / `EUTRA` / `WLAN` | 접속 무선 종류 |
> | 4 | `ambrUl` | integer | O | 0 ~ 4,000,000 (kbps) | 상향 최대 대역 |
> | 5 | `ambrDl` | integer | O | 0 ~ 4,000,000 (kbps) | 하향 최대 대역 |
> | 6 | `allowedNssai` | array | **M** | **1 ~ 8개** | 허용 슬라이스 목록 |
> | 6-1 | └ `sst` | integer | **M** | 0 ~ 255 | 슬라이스 종류 |
> | 6-2 | └ `sd` | string | O | 6자리 16진수 | 슬라이스 구분자 |
> | 7 | `suspended` | boolean | **M** | 기본값 `false` | 정지 여부 |
> | 8 | `lastSeen` | integer | O | UNIX epoch (초) | 마지막 접속 시각 |

### 미션

1. `subs_type.h` — 위 표를 구조체로.
   **Optional 은 전부 `has_` 플래그.**
2. `rat_type_t` enum + `rat_type_to_str()` / `rat_type_from_str()` **쌍**.
3. `subs_profile_init(subs_profile_t *p)` — 안전한 초기 상태로.
4. `subs_profile_validate(const subs_profile_t *p)` — 표의 범위를 **전부** 검사.
   위반 시 어느 필드인지 로그.
5. `subs_profile_dump(const subs_profile_t *p)` — Optional 은 **없으면 `-` 로** 출력.
6. **정렬** — 코드를 치기 **전에** 아래 2장의 "오프셋 손 계산표"로 1차 구조체의 `sizeof` 를 **예측**하고, 다 친 뒤 `sizeof`/`offsetof` 를 찍어 **맞는지 확인.** 그다음 멤버를 재배치해 패딩을 줄이고 `_Static_assert` 로 크기를 고정.

### 확장 요구사항 — **조건부(C) 필드** (같은 레코드에 이어서 붙인다)

> [!QUOTE] 요구사항서 (2차 개정 — 필드 9~12 추가)
>
> | # | 필드 | 타입 | **P** | **조건 (P = C 일 때)** | 값 범위 / 형식 |
> | :--- | :--- | :--- | :---: | :--- | :--- |
> | 9 | `chargingMethod` | enum | **M** | — | `PREPAID` / `POSTPAID` |
> | 10 | `prepaidInfo` | object | **C** | `chargingMethod == PREPAID` | 아래 10-1 |
> | 10-1 | └ `balance` | integer | **M** | — | 0 ~ 40억 (원) |
> | 11 | `postpaidInfo` | object | **C** | `chargingMethod == POSTPAID` | 아래 11-1, 11-2 |
> | 11-1 | └ `billingDay` | integer | **M** | — | 1 ~ 28 (일) |
> | 11-2 | └ `invoiceEmails` | array | O | — | **0 ~ 3개**, 각 63바이트 이하 |
> | 12 | `suspendCause` | string | **C** | `suspended == true` | 63바이트 이하 |

여기 C 가 **세 개**인데 **성격이 다르다.** 먼저 그것부터 가른다.

### 미션 (이어서)

7. **C 판단표**부터 채운다 (코드 치기 전).
   세 C 를 **독립 / 배타 / 묶음 / 최소하나** 중 무엇인지 분류.
8. `suspend_cause` — **C 가 하나뿐**인 경우.
   `has_` + 조건 검사.
   "조건 거짓인데 값이 왔다" 의 **정책을 정하고 한 줄 근거를 적는다.**
9. `prepaid` / `postpaid` — **배타 C**.
   `charging_method` 를 **태그로 재사용**하는 tagged union.
   `subs_profile_get_postpaid()` / `subs_profile_set_postpaid()` **쌍**을 만들고, **구조체 바깥에서 `info` 를 직접 건드리는 코드가 없게** 한다.
10. 확장 때문에 **6번의 `_Static_assert` 가 깨진다.** 새 `sizeof` 를 다시 **예측 → 실측 → 갱신**.
    (깨지는 게 정상이다.
    그게 `_Static_assert` 를 박아 둔 이유다.)

> [!IMPORTANT] `prepaidInfo` 는 값이 **한 개**, `postpaidInfo` 는 **여러 개**다
> 이 둘을 union 에 나란히 넣을 수 있는 이유는 **각자를 자기 구조체로 감쌌기** 때문이다.
> 감싸고 나면 union 입장에선 둘 다 "구조체 하나" 라 똑같이 다룬다.
> **배열의 개수 필드(`email_cnt`)도 그 구조체 안에** 넣는다 — union 밖에 두면 태그가 바뀔 때 엉뚱한 개수를 본다.

### C 판단표 — 채우고 시작한다

| 필드 | 조건 | 다른 C 와 **동시에** 올 수 있나 | 패턴 | 표현 |
| :--- | :--- | :--- | :--- | :--- |
| `suspend_cause` |  |  |  |  |
| `prepaid_info` |  |  |  |  |
| `postpaid_info` |  |  |  |  |

> [!DANGER] 여기서 union 을 잘못 고르면 **에러 없이 데이터가 지워진다**
> union 멤버는 같은 메모리를 겹쳐 쓴다.
> **동시에 올 수 있는 필드**를 union 에 넣으면
> 둘째를 채우는 순간 첫째가 조용히 망가진다.
> 컴파일도 되고 경고도 없다.
> **"정확히 하나만 존재한다"** 고 규격이 보장할 때만 union 이다.
> → 분류 기준 전체는 [데이터 모델 골격 2-8](../../12%20코드%20골격/%5B골격%5D%202.%20데이터%20모델%20골격%20—%20규격서%20I·O%20표를%20구조체로.md)

**에러 코드 규칙 (A)**

| 상황 | 리턴코드 | 로그 |
| :--- | :--- | :--- |
| `p` 가 NULL | `RET_INVALID_ARG` | 가드가 남긴다 |
| 표의 범위·개수 위반 (`nssai_num` 0 / 9, `ambr` 초과 등) | `RET_BAD_FORMAT` | O (어느 필드인지) |
| 조건 참인데 C 필드가 없음 (`suspended` 인데 `suspend_cause` 없음) | `RET_INVALID_ARG` | O |
| `from_str` 에 NULL | `RET_INVALID_ARG` | 가드가 남긴다 |
| `from_str` 에 모르는 문자열 | `RET_OK` + `RAT_UNKNOWN` | WRN (규격에 값이 추가돼도 죽지 않게) |

## 시그니처

```c
const char *rat_type_to_str(rat_type_t v);
int rat_type_from_str(const char *s, rat_type_t *out);
_Static_assert(sizeof(subs_profile_t) == 416, "subs_profile_t layout changed");
_Static_assert(offsetof(subs_profile_t, last_seen) == 0, "8B member must be first");
void subs_profile_init(subs_profile_t *p);
int subs_profile_validate(const subs_profile_t *p);
void subs_profile_dump(const subs_profile_t *p);
const prepaid_info_t *subs_profile_get_prepaid(const subs_profile_t *p);
const postpaid_info_t *subs_profile_get_postpaid(const subs_profile_t *p);
int subs_profile_set_prepaid_balance(subs_profile_t *p, uint32_t balance);
int subs_profile_set_postpaid(subs_profile_t *p, const postpaid_info_t *src);
```

(타입과 상수는 같은 폴더의 `*.h` 에 이미 있다. `solution.c` 에는 함수 본문만 채운다.)

## 입출력 예시

위 "에러" 표와 규칙의 예가 곧 입출력 예시다. 시험(`test.c`)이 대표 입력과 경계를 전부 확인한다.

## 제약

- 공통 헤더(`common.h`, `guard.h`, `log.h`, `safe.h`)의 가드·단일 출구·`goto CLEANUP` 패턴을 쓴다(저장소 `common/`).
- 실패하면 출력 구조체는 0 (쓰레기를 남기지 않는다) 등 표의 규칙을 그대로 지킨다.
- 경고 0, ASan/UBSan/LSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P040` → `PASS`, 실패하면 `FAIL k` 와 어긋난 항목이 출력된다. 힌트: `HINT1.md` → `HINT2.md`.
