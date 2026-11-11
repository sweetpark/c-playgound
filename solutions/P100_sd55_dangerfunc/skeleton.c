/* P100 dangerfunc — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 
#endif
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
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

