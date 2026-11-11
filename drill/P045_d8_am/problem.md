# P045 · d8 — 3GPP — IE 표 한 장을 끝까지

> 예상 시간: 큰 문제: 이틀 이상 나눠서. 이 드릴은 크므로 막히면 HINT 와 NOTE 를 적극적으로 쓰고, 며칠에 나눠 풀어도 된다.

## 요구사항

(변형 A — 가입자 AM 데이터 조회 (SBI 흉내))

> [!QUOTE] 연습용 규격 (단순화)
> **`GET /subscriber-data/{supi}/am-data?plmn-id={plmn}&supported-features={feat}`**
>
> **요청 IE**
>
> | IE | P | Cardinality | Type | 설명 |
> | :--- | :--- | :--- | :--- | :--- |
> | `supi` | **M** | 1 | string | `imsi-` + 숫자 15자 |
> | `plmnId` | **C** | 0..1 | object | **로밍 가입자면 필수**. `{mcc:3자리, mnc:2~3자리}` |
> | `supportedFeatures` | O | 0..1 | string | 16진수 문자열, 비트 단위 기능 협상 |
>
> **응답 IE (200 OK)**
>
> | IE | P | Cardinality | Type | 설명 |
> | :--- | :--- | :--- | :--- | :--- |
> | `gpsi` | O | 0..1 | string | `msisdn-` + 숫자 |
> | `subscribedUeAmbr` | **C** | 0..1 | object | **가입자가 정지 상태가 아니면 필수**. `{uplink, downlink}` (`"100 Mbps"` 형식) |
> | `nssai` | **M** | 1 | object | `{defaultSingleNssais: 1..8, singleNssais: 0..8}` |
> | `ratRestrictions` | O | 0..8 | array(enum) | `NR` / `EUTRA` / `WLAN` |
> | `supportedFeatures` | **C** | 0..1 | string | **요청에 왔으면 필수** |
>
> **에러 응답 (ProblemDetails)**
>
> | 상황 | HTTP | cause |
> | :--- | :--- | :--- |
> | 필수 IE 없음 | 400 | `MANDATORY_IE_MISSING` |
> | 형식 위반 | 400 | `INVALID_MSG_FORMAT` |
> | 가입자 없음 | 404 | `USER_NOT_FOUND` |
> | 내부 오류 | 500 | `INTERNAL_ERROR` |
> | 미지원 기능 | 501 | `UNSUPPORTED_FEATURE` |

### 미션

1. 요청·응답 **표 3장**을 직접 다시 만든다 (위 표를 그대로 베끼지 말고 **내 양식**으로).
2. `am_data_req_t` / `am_data_rsp_t` 구조체 (D2 규칙 그대로).
3. `"100 Mbps"` 같은 **문자열 비트레이트**를 `uint64_t bps` 로 변환하는 함수 + 역변환.
   단위: `Kbps`/`Mbps`/`Gbps`.
4. `mcc`/`mnc` 문자열 → `plmn_t` (mnc 2자리·3자리 **둘 다** 처리).
5. `am_data_validate()` — **C(Conditional) 조건을 규격 문구 주석과 함께** 검사.
6. `ret_to_problem()` — `ret_t` → `{status, cause}` 매핑 **한 곳**.
7. 모르는 최상위 필드는 **원문 보존**해서 응답에 echo.

### 확장 요구사항 — **C 가 여럿인 IE** (규격 개정 2, 응답에 이어 붙인다)

> [!QUOTE] 연습용 규격 (단순화) — 응답 IE 추가
>
> | IE | P | Cardinality | Type | 설명 |
> | :--- | :--- | :--- | :--- | :--- |
> | `accessType` | **M** | 1 | enum | `3GPP_ACCESS` / `NON_3GPP_ACCESS` |
> | `gnbInfo` | **C** | 0..1 | object | **`accessType == 3GPP_ACCESS` 일 때 present.** `{gnbId: uint32, tac: uint16}` |
> | `wlanInfo` | **C** | 0..1 | object | **`accessType == NON_3GPP_ACCESS` 일 때 present.** `{ssid: 1~32자, bssid: 17자}` |
> | `ueIdentity` | **M** | 1 | **oneOf** | `{"supi": string}` **또는** `{"gpsi": string}` — **정확히 하나** |
> | `ueIpv4` | **C** | 0..1 | string | **`anyOf`** (아래 `ueIpv6` 와) — **둘 중 하나 이상** present |
> | `ueIpv6` | **C** | 0..1 | string | 위와 동일. 둘 다 와도 정상 |

### 미션 (이어서)

8. **C 판단표**부터 채운다 (코드 치기 전).
   위 5개 C 필드를 **독립 / 배타(oneOf) / 최소하나(anyOf) / 묶음** 중 무엇인지 분류하고, **원문 단어**(`oneOf`, `anyOf`, "일 때")를 근거 칸에 적는다.
9. `access_info_t` — **판별자가 표에 이미 있는** 배타 C. `accessType` 을 **태그로 재사용**하는 tagged union.
10. `ue_identity_t` — **판별자가 표에 없는** `oneOf`.
    `kind` enum 을 **새로 만든다.** 파싱은 "어느 키가 왔나"로 태그를 정하고 **0개·2개 이상 모두 400.**
11. `ue_ip` — `anyOf`.
    **union 이 아니라** 각자 `has_` + `has_v4 + has_v6 >= 1`.
    **왜 union 이면 안 되는지 시험으로 증명**한다 (둘 다 온 요청이 둘 다 살아 있어야 한다).
12. 생성(직렬화)도 **태그로 갈라서** — `kind == NONE` 인 채로 응답을 만들려 하면 `RET_INVALID_ARG`.

> [!DANGER] `oneOf` 와 `anyOf` 를 **원문 단어로** 구분한다
> 둘 다 "C 가 두 개" 처럼 보이지만 **동시에 올 수 있는지**가 반대다.
> `anyOf` 를 union 에 넣으면 둘째를 파싱하는 순간 첫째가 **에러 없이 지워진다.**
> 헷갈리면 `has_` 로 두는 쪽이 안전하다.
> → [6번 템플릿 2-4](../../12%20코드%20골격/%5B골격%5D%206.%203GPP%20규격%20→%20C%20코드%20변환%20골격.md), [2번 템플릿 2-8](../../12%20코드%20골격/%5B골격%5D%202.%20데이터%20모델%20골격%20—%20규격서%20I·O%20표를%20구조체로.md)

### C 판단표 — 채우고 시작한다

| 필드 | 조건 / 원문 단어 | 다른 C 와 **동시에** 올 수 있나 | 패턴 | 표현 |
| :--- | :--- | :--- | :--- | :--- |
| `gnbInfo` |  |  |  |  |
| `wlanInfo` |  |  |  |  |
| `ueIdentity` (`supi` / `gpsi`) |  |  |  |  |
| `ueIpv4` |  |  |  |  |
| `ueIpv6` |  |  |  |  |

**공통 타입 `am_types.h` (A, B, C 가 같이 쓴다)**

| 이름 | 모양 |
| :--- | :--- |
| 상수 | `SUPI_SIZE` 24, `GPSI_SIZE` 24, `SSID_LEN_MAX` 32, `BSSID_SIZE` 18, `IPV4_STR_SIZE` 16, `IPV6_STR_SIZE` 46, `NSSAI_NUM_MAX` 8 |
| `plmn_t` | `char mcc[4]`, `char mnc[4]`, `uint8_t mnc_len` (2 또는 3) |
| `problem_t` | `int status`, `const char *cause` |
| `am_data_req_t` | `char supi[SUPI_SIZE]`, `bool has_plmn` + `plmn_t plmn`, `bool has_features` + `char features[17]` |
| `am_data_rsp_t` | `bool suspended`(내부 상태), `has_gpsi` + `gpsi`, `has_ue_ambr` + `ue_ambr {uint64_t ul_bps, dl_bps}`, `nssai {default_num, default_sst[8], single_num, single_sst[8]}`, `rat_num` + `rat[8]` (D2 의 `rat_type_t`: `RAT_NR` / `RAT_EUTRA` / `RAT_WLAN`), `has_features` + `features[17]`, `unknown[8]` + `unknown_num` (`{char key[32]; char raw[256];}`), (확장) `access_info_t access`, `ue_identity_t ue_id`, `ue_ip_t ue_ip` |

**함수 (A)**

| 함수 | 요구사항 |
| :--- | :--- |
| `int am_req_parse(const char *url, am_data_req_t *out)` | `/subscriber-data/{supi}/am-data?plmn-id={plmn}&supported-features={feat}` 를 파싱한다.<br>`supi` 는 `imsi-` + 숫자 15자.<br>`plmn-id` 는 숫자 5~6자 (mcc 3 + mnc 2~3).<br>`supported-features` 는 16진수 문자열 (최대 16자).<br>**없는 쿼리는 `has_` 를 끈다.** |
| `int am_req_validate(const am_data_req_t *req, bool roaming)` | `roaming` 인데 `plmnId` 가 없으면 `RET_INVALID_ARG` |
| `int am_rsp_parse(json_t *root, am_data_rsp_t *out)` | 응답 JSON → 구조체.<br>`nssai` 가 없으면 `RET_INVALID_ARG`, 타입이 틀리면 `RET_BAD_FORMAT`.<br>**모르는 최상위 키는 `unknown[]` 에 원문(JSON 문자열)으로 보관**한다 (8개 초과는 WRN 후 버림). |
| `int am_rsp_build(const am_data_rsp_t *in, json_t **out)` | 구조체 → JSON.<br>**보관한 모르는 키를 그대로 되돌려 넣는다 (echo).**<br>`subscribedUeAmbr` 는 `"100 Mbps"` 형식으로 쓴다. |
| `int am_data_rsp_validate(const am_data_rsp_t *rsp, const am_data_req_t *req)` | C 조건을 **규격 문구 주석**과 함께 검사한다. |
| `int bitrate_from_str(const char *s, uint64_t *bps)` | `"100 Mbps"` / `"1.5 Gbps"` / `"64 Kbps"` / `"10 bps"` → bps.<br>단위는 **대소문자 무시**, 실패하면 `*bps = 0`. |
| `int bitrate_to_str(uint64_t bps, char *buf, size_t n)` | 역변환.<br>**1000 이상이면 가장 큰 단위**로, 소수는 필요한 만큼만 (`1.5 Gbps`, `64 Kbps`), 1000 미만은 `"999 bps"`.<br>버퍼가 모자라면 `RET_TOO_SMALL`. |
| `int plmn_parse(const char *mcc, const char *mnc, plmn_t *out)` | mnc 가 **2자리·3자리 둘 다** 가능.<br>`mnc_len` 을 따로 보관한다 (`"01"` 과 `"001"` 은 다른 사업자). |
| `int plmn_to_strs(const plmn_t *p, char mcc[4], char mnc[4])` | 앞자리 0 을 **그대로** 되돌린다. |
| `problem_t ret_to_problem(int ret)` | `ret_t` → `{status, cause}` 매핑 **한 곳** |
| `int features_from_hex(const char *s, uint32_t *out)` | 16진수 문자열 → 마스크 (최대 8자리).<br>빈 문자열·16진수가 아닌 문자 → `RET_BAD_FORMAT` |
| `int features_to_hex(uint32_t v, char *buf, size_t n)` | 소문자, 앞의 0 없음 (`0` 은 `"0"`) |
| `uint32_t features_negotiate(uint32_t peer, uint32_t mine)` | `peer & mine` — **상대가 모르는 비트는 끈다.** |

**에러 코드 → HTTP 매핑 (한 함수에만)**

| `ret_t` | HTTP | cause |
| :--- | :--- | :--- |
| `RET_OK` | 200 | (NULL) |
| `RET_INVALID_ARG` | 400 | `MANDATORY_IE_MISSING` |
| `RET_BAD_FORMAT` | 400 | `INVALID_MSG_FORMAT` |
| `RET_NOT_FOUND` | 404 | `USER_NOT_FOUND` |
| `RET_NOT_SUPPORTED` | 501 | `UNSUPPORTED_FEATURE` |
| `RET_TIMEOUT` | 504 | `TIMEOUT` |
| 그 밖의 모든 값 | 500 | `INTERNAL_ERROR` |

**확장 함수 (A, C 변형 요구사항 5개 C 필드)**

| 함수 | 요구사항 |
| :--- | :--- |
| `int access_info_parse(json_t *root, access_info_t *out)` | `accessType` 이 **태그**.<br>`accessType` 이 없으면 `RET_INVALID_ARG`, 모르는 값이면 `RET_BAD_FORMAT`.<br>**조건 참인데 해당 arm 이 없으면 `RET_INVALID_ARG`**.<br>반대편 arm 이 같이 오면 **무시 + WRN**. |
| `int ue_identity_parse(json_t *obj, ue_identity_t *out)` | `oneOf` — **정확히 하나**.<br>0개도 2개도 `RET_BAD_FORMAT`.<br>`kind` 는 **새로 만든다** (`UEID_NONE=0` / `SUPI` / `GPSI`). |
| `int ue_identity_build(const ue_identity_t *id, json_t *obj)` | `kind == NONE` 이면 `RET_INVALID_ARG` (규격 위반 응답을 만들지 않는다). |
| `int ue_ip_validate(const ue_ip_t *p)` | `anyOf` — **하나 이상**.<br>0개만 `RET_BAD_FORMAT`, **둘 다 와도 정상**이고 **둘 다 보존**한다. |

## 시그니처

```c
int bitrate_from_str(const char *s, uint64_t *out_bps);
int bitrate_to_str(uint64_t bps, char *buf, size_t n);
int plmn_parse(const char *mcc, const char *mnc, plmn_t *out);
int plmn_to_strs(const plmn_t *p, char mcc[4], char mnc[4]);
problem_t ret_to_problem(int ret);
int features_from_hex(const char *s, uint32_t *out);
int features_to_hex(uint32_t v, char *buf, size_t n);
uint32_t features_negotiate(uint32_t peer, uint32_t mine);
int am_req_parse(const char *url, am_data_req_t *out);
int am_req_validate(const am_data_req_t *req, bool roaming);
int am_rsp_parse(json_t *root, am_data_rsp_t *out);
int am_rsp_build(const am_data_rsp_t *in, json_t **out);
int am_data_rsp_validate(const am_data_rsp_t *rsp, const am_data_req_t *req);
int access_info_parse(json_t *root, access_info_t *out);
int ue_identity_parse(json_t *obj, ue_identity_t *out);
int ue_identity_build(const ue_identity_t *id, json_t *obj);
int ue_ip_validate(const ue_ip_t *p);
```

(타입과 상수는 같은 폴더의 `*.h` 에 이미 있다. `solution.c` 에는 함수 본문만 채운다.)

## 입출력 예시

위 "에러" 표와 규칙의 예가 곧 입출력 예시다. 시험(`test.c`)이 대표 입력과 경계를 전부 확인한다.

## 제약

- 공통 헤더(`common.h`, `guard.h`, `log.h`, `safe.h`)의 가드·단일 출구·`goto CLEANUP` 패턴을 쓴다(`drill/common/`).
- 실패하면 출력 구조체는 0 (쓰레기를 남기지 않는다) 등 표의 규칙을 그대로 지킨다.
- 경고 0, ASan/UBSan/LSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P045` → `PASS`, 실패하면 `FAIL k` 와 어긋난 항목이 출력된다. 힌트: `HINT1.md` → `HINT2.md`.
