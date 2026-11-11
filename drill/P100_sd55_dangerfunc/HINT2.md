# HINT2 (막혔을 때만)
```
int demonstrate_vfork_bad(void)
{
    /* 1. int shared_var = 100
     * 2. pid = vfork()   ← 자식이 부모의 주소 공간을 빌려 쓴다
     * 3. pid == 0 이면 shared_var = 999, _exit(0)
     * 4. waitpid(pid, NULL, 0)
     * 5. return shared_var   ← 부모 값이 999 로 오염 */
}

int demonstrate_fork_good(void)
{
    /* [ ] 1. int shared_var = 100 */

    /* [ ] 2. pid = fork()   ← vfork 대신 이 한 단어만 바뀐다 */

    /* [ ] 3. pid == 0 이면 (자식)
     *        shared_var = 999   ← 자식만의 복사본
     *        _exit(0) */

    /* [ ] 4. waitpid(pid, NULL, 0) */

    /* [ ] 5. return shared_var   ← 부모 값은 그대로 100 */
}
```
