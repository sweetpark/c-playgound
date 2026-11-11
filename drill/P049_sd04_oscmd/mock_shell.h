#ifndef MOCK_SHELL_H
#define MOCK_SHELL_H
/* 실제로 실행하지 않고 ';' 로 구분되는 "명령 개수"만 센다.
   진짜 셸이라면 이 개수만큼 프로세스가 실행된다. */
int mock_shell_command_count(const char *cmdline);
#endif
