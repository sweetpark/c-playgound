#include <signal.h>
#include <stddef.h>
#include <string.h>

/* P022 sig_counter — 요구사항은 problem.md */

/* 핸들러가 올리는 카운터 (타입에 주의) */
static int g_count; /* TODO: 시그널 핸들러와 공유하는 변수의 올바른 타입은? */

int sigc_install(int signo)
{
	(void)signo;
	/* TODO */
	return -1;
}

int sigc_uninstall(int signo)
{
	(void)signo;
	/* TODO */
	return -1;
}

int sigc_count(void)
{
	/* TODO */
	return -1;
}

void sigc_reset(void)
{
	(void)g_count;
	/* TODO */
}
