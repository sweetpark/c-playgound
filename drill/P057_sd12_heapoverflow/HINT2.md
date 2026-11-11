# HINT2 (막혔을 때만)
```
void heap_copy_bad(const char *input)
{
    /* 1. char *dest = malloc(BUFSIZE)   (BUFSIZE 10)
     * 2. NULL 이면 return
     * 3. strcpy(dest, input)   ← 길이 검사 없음
     * 4. free(dest) */
}

int heap_copy_good(const char *input, char **out)
{
    /* [ ] 1. 길이 구하기
     *        size_t len = strlen(input) */

    /* [ ] 2. 길이 검사 (할당 전)
     *        len >= BUFSIZE → return -1 */

    /* [ ] 3. 할당
     *        *out = malloc(BUFSIZE)
     *        NULL 이면 → return -1 */

    /* [ ] 4. strncpy(*out, input, BUFSIZE - 1) */

    /* [ ] 5. (*out)[BUFSIZE - 1] = '\0' */

    /* [ ] 6. return 0   (해제는 호출자 책임) */
}
```
