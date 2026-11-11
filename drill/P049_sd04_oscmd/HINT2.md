# HINT2 (막혔을 때만)
```
int build_and_count_bad(const char *userArg)
{
    /* 1. char cmd[256]
     * 2. snprintf(cmd, sizeof(cmd), "cat %s", userArg)   ← 검증 없이 조립
     * 3. return mock_shell_command_count(cmd) */
}

int build_and_count_good(const char *userArg)
{
    /* [ ] 1. 지역변수
     *        char cmd[256] */

    /* [ ] 2. 위험 문자 검사 (조립 전)
     *        strpbrk(userArg, 셸 메타문자) != NULL → return -1
     *        셸 메타문자: ; | & ` $ \ " '
     *        '.' 은 넣지 않는다 (확장자에 쓰인다) */

    /* [ ] 3. snprintf 로 "cat %s" 조립 */

    /* [ ] 4. return mock_shell_command_count(cmd) */
}
```
