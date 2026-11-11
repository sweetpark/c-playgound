# HINT2 (막혔을 때만)
```
void render_title_bad(const char *title, char *out, size_t out_size)
{
    /* 1. snprintf(out, out_size, "<FONT SIZE=+1><STRONG>%s</STRONG></FONT>", title)
     *    ← title 을 그대로 삽입 */
}

void render_title_good(const char *title, char *out, size_t out_size)
{
    /* [ ] 1. 지역변수
     *        char escaped[512] */

    /* [ ] 2. escape_html(in, out, out_size) 를 먼저 만든다
     *        in 을 한 글자씩 한 번만 훑는다
     *          '<' → "&lt;"    '>' → "&gt;"
     *          '&' → "&amp;"   '"' → "&quot;"
     *          그 외 → 그대로
     *        치환 문자열을 쓰기 전에 남은 공간을 확인한다 (j + rl >= out_size 이면 중단)
     *        끝에 '\0' */

    /* [ ] 3. escape_html(title, escaped, sizeof(escaped)) */

    /* [ ] 4. snprintf 로 HTML 조립 (title 대신 escaped) */
}
```
