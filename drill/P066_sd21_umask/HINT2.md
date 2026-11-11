# HINT2 (막혔을 때만)
```
int create_file_bad(const char *path)
{
    /* 1. old = umask(0)   ← 모두 허용
     * 2. fd = open(path, O_WRONLY|O_CREAT|O_TRUNC, 0666)
     * 3. umask(old)
     * 4. 실패면 -1, 성공이면 close 후 get_file_mode(path) */
}

int create_file_good(const char *path)
{
    /* [ ] 1. 이전 umask 를 저장하고 최소 권한으로 바꾼다
     *        mode_t old = umask(077) */

    /* [ ] 2. fd = open(path, O_WRONLY|O_CREAT|O_TRUNC, 0666)
     *        mode 인자는 Bad 와 똑같다 (실제 권한 = mode & ~umask) */

    /* [ ] 3. umask(old)   ← 즉시 복원 */

    /* [ ] 4. fd < 0 이면 → return -1 */

    /* [ ] 5. close(fd) */

    /* [ ] 6. return get_file_mode(path)
     *        stat 의 st_mode & 0777, 실패면 -1 */
}
```
