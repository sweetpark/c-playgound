#include "mock_shell.h"

int mock_shell_command_count(const char *cmdline)
{
    int count = 1;
    const char *p;
    for (p = cmdline; *p; p++) {
        if (*p == ';') count++;
    }
    return count;
}
