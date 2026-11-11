# HINT2 (막혔을 때만)
```
/* subs_type.h */
/* [ ] 1. 인클루드 가드 SUBS_TYPE_H, include: <stdbool.h> <stdint.h> <time.h> */
/* [ ] 2. 상수: SUPI_LEN_MAX 15 / NICKNAME_LEN_MAX 31 / SD_LEN 6 / NSSAI_NUM_MAX 8 / AMBR_KBPS_MAX 4000000 */
/* [ ] 3. rat_type_t: RAT_UNKNOWN = 0 ... RAT_END   + to_str / from_str 선언 */
/* [ ] 4. snssai_t: sst(M), has_sd + sd[SD_LEN + 1]   (has_ 는 값 바로 위) */
/* [ ] 5. (1차) subs_profile_t — 규격서 표 순서대로
 *        M : supi[+1], suspended
 *        O : nickname, rat_type, ambr_ul, ambr_dl, last_seen   → 각각 has_ 를 값 바로 위
 *        배열 : nssai[8] + nssai_num (개수는 배열 바로 아래)
 *        time_t 는 64비트 (int 금지) */
/* [ ] 6. (손 계산) 오프셋 표를 채우고 예측 sizeof 를 적는다.  ← 코드로 찍기 전에 */
/* [ ] 7. (2차) 재배치
 *        8B (time_t) → 4B (uint32_t, enum) → 1B (bool, has_ 를 한곳에) → char[] / 정렬 1 구조체
 *        짝이 떨어진 has_ 는 그룹 주석에 "어느 값의 것인지" 적는다
 *        _Static_assert(sizeof(subs_profile_t) == 예측값) */
/* [ ] 8. 확장 (C 필드)
 *        charge_method_t (UNKNOWN = 0 / PREPAID / POSTPAID / END)   ← 태그
 *        prepaid_info_t { balance }
 *        postpaid_info_t { billing_day, email_cnt, emails[3][64] }  ← 개수는 이 안에
 *        union info { prepaid; postpaid; }
 *        has_suspend_cause + suspend_cause[64]                      ← union 에 넣지 않는다 (독립 C)
 *        새 sizeof 를 다시 예측 → 실측 → _Static_assert 갱신 */
/* [ ] 9. 함수 선언: init / validate / dump / get_prepaid / get_postpaid / set_prepaid_balance / set_postpaid */

/* subs_type.c */
/* [ ] 1. g_rat_str[RAT_END] — 지정 초기화자 ([RAT_NR] = "NR" ...) */
/* [ ] 2. rat_type_to_str: 범위 밖이면 "UNKNOWN" */
/* [ ] 3. rat_type_from_str
 *        가드(s, out) → *out = RAT_UNKNOWN → 표 순회
 *        못 찾아도 에러가 아니다: WRN 로그 후 RET_OK */
/* [ ] 4. subs_profile_init: NULL 이면 return / memset / 기본값(suspended = false) 명시 */
/* [ ] 5. V_FAIL(fmt, ...) 지역 매크로: LOG_ERR 후 RET_BAD_FORMAT 리턴 */
/* [ ] 6. subs_profile_validate (위에서 아래로)
 *        supi      : 1~15자, 숫자만
 *        nssai_num : 1~8, sd 가 있으면 정확히 6자
 *        O 필드    : has_ 가 켜진 것만 범위 검사
 *        C suspend_cause : suspended 이면 has_ + 비어 있지 않음 (없으면 RET_INVALID_ARG)
 *                          아니면서 값이 왔으면 정책 선택 + 근거 주석
 *        charging_method : switch (태그) — default 는 UNKNOWN 을 반려, 가리키는 arm 만 검사 */
/* [ ] 7. subs_profile_dump: O 필드는 없으면 "-" */
/* [ ] 8. set_* : ① 태그 대입 → ② memset(&info) → ③ arm 채우기      (이 순서)
 *        get_* : 태그가 다르면 NULL */
```
