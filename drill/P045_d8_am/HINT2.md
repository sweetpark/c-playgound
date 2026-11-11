# HINT2 (막혔을 때만)
```
/* am_types.h  — 표 3장을 구조체로 (D2 규칙) */
/* [ ] 1. 상수 #define (N 은 전부 #define)
 * [ ] 2. plmn_t { mcc[4], mnc[4], mnc_len }                    ← mnc_len 을 잃으면 복원 불가
 * [ ] 3. access_type_t (UNKNOWN = 0 / 3GPP / NON_3GPP / END)    ← 표에 판별자가 있으니 태그로 재사용
 *        access_info_t { type; union { gnb_info_t gnb; wlan_info_t wlan; } u; }
 * [ ] 4. ue_id_kind_t (NONE = 0 / SUPI / GPSI / END)            ← 표에 판별자가 없으니 새로 만든다
 *        ue_identity_t { kind; union { char supi[..]; char gpsi[..]; } u; }
 * [ ] 5. ue_ip_t { has_v4 + v4, has_v6 + v6 }                   ← anyOf 라서 union 이 아니다
 * [ ] 6. am_data_req_t / am_data_rsp_t / problem_t */

/* features.c / bitrate.c / plmn.c / problem.c */
/* [ ] 1. bitrate_from_str
 *        가드(CHK_STR, CHK_PTR) → *bps = 0
 *        sscanf("%lf %7s")   ← 폭 제한을 꼭 둔다
 *        단위: strcasecmp 로 bps / Kbps / Mbps / Gbps → 배수
 *        val < 0 이거나 너무 크면 BAD_FORMAT, 곱해서 대입 */
/* [ ] 2. bitrate_to_str: 단위 고르기 → 소수 필요한 만큼만 → snprintf (잘리면 TOO_SMALL) */
/* [ ] 3. plmn_parse: mcc 는 숫자 3자리, mnc 는 숫자 2 또는 3자리 → 문자열 그대로 복사 + mnc_len
 *        plmn_to_strs: 그대로 되돌린다 */
/* [ ] 4. ret_to_problem: switch 한 곳, default 는 500 */
/* [ ] 5. features_from_hex (strtoul 쓰지 말고 한 글자씩) / features_to_hex / features_negotiate */

/* am_req.c */
/* [ ] 1. am_req_parse(url, out)
 *        가드 → memset
 *        접두 "/subscriber-data/" 확인 → supi 구간 추출 → "/am-data" 확인
 *        supi 구간이 비었으면 INVALID_ARG, 형식("imsi-" + 숫자 15자)이 아니면 BAD_FORMAT
 *        '?' 뒤를 '&' 로 나눠 key=value 처리 (plmn-id, supported-features, 모르는 키는 무시)
 *        plmn-id: 숫자 5~6자 → mcc 3 + mnc 나머지 → plmn_parse
 *        supported-features: features_from_hex 로 검증 후 문자열 보관 */
/* [ ] 2. am_req_validate: roaming && !has_plmn → INVALID_ARG */

/* am_rsp.c */
/* [ ] 1. am_data_rsp_validate (규격 문구를 주석으로 박는다)
 *        M  : nssai.default_num 이 1~8
 *        C  : "정지 상태가 아니면 subscribedUeAmbr 필수"
 *        C  : "요청에 supportedFeatures 가 있었으면 응답에도 필수"
 *        O  : gpsi 가 있으면 "msisdn-" 접두 확인 */
/* [ ] 2. am_rsp_parse
 *        nssai 없으면 INVALID_ARG, 타입이 틀리면 BAD_FORMAT
 *        알려진 키는 구조체로, 그 밖의 키는 json_dumps 로 원문 문자열 보관 (8개 초과는 WRN) */
/* [ ] 3. am_rsp_build: 구조체 → JSON, 보관한 키를 그대로 되돌린다, ambr 는 bitrate_to_str */

/* access.c */
/* [ ] 1. access_info_parse
 *        accessType 문자열 → 없으면 INVALID_ARG
 *        "3GPP_ACCESS"      : gnbInfo 가 없으면 INVALID_ARG / wlanInfo 가 같이 오면 WRN 후 무시
 *        "NON_3GPP_ACCESS"  : 반대
 *        그 외 문자열       : BAD_FORMAT
 *        태그(type)를 정하고 그 arm 만 채운다 */
/* [ ] 2. ue_identity_parse: n_hit = (supi 있음) + (gpsi 있음), n_hit != 1 이면 BAD_FORMAT → 온 쪽만 길이 검사 후 복사, kind 설정 */
/* [ ] 3. ue_identity_build: switch (kind) — SUPI / GPSI, default 는 INVALID_ARG */
/* [ ] 4. ue_ip_validate: has_v4 + has_v6 < 1 이면 BAD_FORMAT */
```
