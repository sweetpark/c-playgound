/* P049 oscmd — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include <stdio.h>
#include <string.h>
#include "mock_shell.h"
#include "cmd_builder.h"

int build_and_count_bad(const char *userArg)
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "cat %s", userArg);
    return mock_shell_command_count(cmd);
}

/* 셸 메타문자만 검사한다. '.' 은 뺀다 — 파일 확장자에 정상적으로 쓰이기 때문이다. */
int build_and_count_good(const char *userArg)
{
	(void)userArg;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

