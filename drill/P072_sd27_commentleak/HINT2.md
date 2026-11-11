# HINT2 (막혔을 때만)
```
int release_ok_bad(const char *source_code)
{
    /* 1. (void)source_code
     * 2. return 1   ← 검사 단계 자체가 없다 */
}

int release_ok_good(const char *source_code)
{
    /* [ ] 1. find_secret_in_comments(source_code) 를 먼저 만든다
     *        "/*" 부터 "*/" 까지 블록 주석을 하나씩 찾는다
     *        각 주석을 buf[512] 에 복사한다 (길이 상한, 널 종료)
     *        KEYWORDS 중 하나라도 strcasestr 로 찾으면 → return 1
     *        끝까지 없으면 → return 0
     *        KEYWORDS: password, passwd, secret, apikey, api_key */

    /* [ ] 2. return find_secret_in_comments(source_code) == 0 */
}
```
