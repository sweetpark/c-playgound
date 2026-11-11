# HINT2 (막혔을 때만)
```
int msisdn_normalize(const msisdn_req_t *in, msisdn_rsp_t *out)
{
    /* [ ] 1. 지역변수 (전부 선언부에서 초기화)
     *        digits[64], cc_str[8], n / i / cc_len = 0, has_plus = 0, nsn = NULL */

    /* [ ] 2. 가드 (이 순서 그대로)
     *        CHK_PTR(in)
     *        CHK_PTR(out)
     *        CHK_RANGE(in->def_cc, 1, 999) */

    /* [ ] 3. out 과 digits 를 0 으로 초기화 */

    /* [ ] 4. in_raw 를 한 글자씩 본다
     *        루프 상한: i < sizeof(in->in_raw) && in_raw[i] != '\0'   ← 널 종단을 믿지 않는다
     *
     *        - 허용 문자(0-9 - 공백 +)가 아니면
     *            → 로그, RET_BAD_FORMAT
     *        - '+' 이면
     *            i != 0 이면 → 로그, RET_BAD_FORMAT
     *            아니면 has_plus = 1, 다음 글자
     *        - '-' 또는 공백이면
     *            → 건너뛴다
     *        - 숫자이면
     *            digits[n++] = c  (digits 가 가득 차면 RET_BAD_FORMAT)
     *
     *        루프 뒤 n == 0 이면 → 로그, RET_BAD_FORMAT */

    /* [ ] 5. cc_str = def_cc 를 문자열로 (snprintf)
     *        cc_len = 그 길이 */

    /* [ ] 6. nsn 시작 위치를 정한다
     *
     *        has_plus 이면
     *            digits 가 cc_str 로 시작하지 않으면 → 로그, RET_NOT_SUPPORTED
     *            시작하면 nsn = digits + cc_len
     *        digits[0] == '0' 이면
     *            nsn = digits + 1   (선행 0 제거)
     *        이미 cc 로 시작하면
     *            nsn = digits + cc_len
     *        그 외
     *            nsn = digits
     *
     *        nsn 이 빈 문자열이면 → 로그, RET_BAD_FORMAT */

    /* [ ] 7. cc_len + strlen(nsn) > 15 이면 → 로그, RET_BAD_FORMAT */

    /* [ ] 8. out 채우기: cc, len, nsn, e164 (cc_str + nsn)
     *        길이를 검증한 뒤 memcpy 로 복사한다 (strcpy 금지) */

    /* [ ] 9. return RET_OK  ← 성공은 맨 끝 한 곳 */
}
```
