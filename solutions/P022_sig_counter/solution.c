#include <signal.h>
#include <stddef.h>
#include <string.h>

/* P022 sig_counter — 모범답안 */
static volatile sig_atomic_t g_count;       /* 핸들러와 공유: volatile sig_atomic_t */

static void on_signal(int signo)
{
	(void)signo;
	g_count++;                              /* 핸들러는 플래그/카운터만. printf/free/malloc 금지 */
}

int sigc_install(int signo)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = on_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;               /* signal() 대신 sigaction: 동작이 시스템마다 다르지 않다 */
	if (sigaction(signo, &sa, NULL) != 0)
		return -1;
	return 0;
}

int sigc_uninstall(int signo)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	return sigaction(signo, &sa, NULL) == 0 ? 0 : -1;
}

int sigc_count(void)
{
	return (int)g_count;
}

void sigc_reset(void)
{
	g_count = 0;
}
