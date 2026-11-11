# HINT2 (막혔을 때만)
```
int file_operation_bad(const char *path, const char *content)
{
    /* 1. access(path, W_OK) != 0 이면 return -1     ← 검사 시점(TOC)
     * 2. f = fopen(path, "w+")                       ← 사용 시점(TOU), 같은 "이름" 을 다시 연다
     * 3. fputs(content, f), fclose(f), return 0 */
}

int file_operation_good(const char *path, const char *content)
{
    /* [ ] 1. 생성과 존재 확인을 **한 번에**
     *        fd = open(path, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR)
     *        fd == -1 이면 → return -1   (이미 있으면 심볼릭 링크여도 무조건 실패) */

    /* [ ] 2. fchmod(fd, S_IRUSR | S_IWUSR)
     *        실패 → close(fd), return -1 */

    /* [ ] 3. f = fdopen(fd, "w")
     *        실패 → close(fd), return -1 */

    /* [ ] 4. fputs(content, f), fclose(f) */

    /* [ ] 5. return 0 */
}
```
