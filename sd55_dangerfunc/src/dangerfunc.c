#define _DEFAULT_SOURCE
#include <unistd.h>
#include <sys/wait.h>
#include "dangerfunc.h"

int demonstrate_vfork_bad(void)
{
    int shared_var = 100;
    pid_t pid = vfork();
    if (pid == 0) {
        shared_var = 999;      /* 자식이 부모의 스택 변수를 직접 변경 -- 부모 공간을 빌려 씀 */
        _exit(0);
    }
    waitpid(pid, NULL, 0);
    return shared_var;          /* vfork 이후 부모 값이 오염됨 */
}

int demonstrate_fork_good(void)
{
    int shared_var = 100;
    pid_t pid = fork();
    if (pid == 0) {
        shared_var = 999;       /* 자식만의 독립된 복사본을 변경 -- 부모는 영향 없음 */
        _exit(0);
    }
    waitpid(pid, NULL, 0);
    return shared_var;           /* 부모 값은 그대로 100 */
}
