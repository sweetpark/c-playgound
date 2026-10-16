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
    char cmd[256];
    if (strpbrk(userArg, ";|&`$\\\"'") != NULL) return -1;
    snprintf(cmd, sizeof(cmd), "cat %s", userArg);
    return mock_shell_command_count(cmd);
}
