#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>

#include "drill_test.h"
#include "solution.c"

static void noop(int s) { (void)s; }

static int is_blocked(int signo)
{
	sigset_t cur;
	sigprocmask(SIG_SETMASK, NULL, &cur);
	return sigismember(&cur, signo);
}

/* 100ms 뒤 부모에게 SIGUSR2 를 보내는 자식 */
static pid_t spawn_sender(void)
{
	pid_t pid = fork();
	if (pid == 0) {
		struct timespec ts = {0, 100000000L};
		nanosleep(&ts, NULL);
		kill(getppid(), SIGUSR2);
		_exit(0);
	}
	return pid;
}

int main(void)
{
	struct sigaction sa;
	sigset_t set;
	pid_t pid;
	int st;

	/* 구현이 틀려서 시그널이 그냥 도착해도 프로세스가 죽지 않도록 */
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = noop;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR2, &sa, NULL);

	/* 1) 대기 도중 도착 */
	CHECK(!is_blocked(SIGUSR2));
	pid = spawn_sender();
	CHECK(pid > 0);
	CHECK(wait_signal(SIGUSR2, 3000) == 1);
	CHECK(!is_blocked(SIGUSR2));           /* 호출 전 마스크 복원 (원래 안 막혀 있었다) */
	waitpid(pid, &st, 0);

	/* 2) 시간 초과 */
	CHECK(wait_signal(SIGUSR2, 200) == 0);
	CHECK(!is_blocked(SIGUSR2));

	/* 3) 호출 전에 이미 도착해 있던(막혀서 보류 중인) 시그널도 놓치지 않는다 */
	sigemptyset(&set);
	sigaddset(&set, SIGUSR2);
	sigprocmask(SIG_BLOCK, &set, NULL);
	raise(SIGUSR2);
	CHECK(wait_signal(SIGUSR2, 1000) == 1);
	CHECK(is_blocked(SIGUSR2));            /* 원래 막혀 있었으면 막힌 채로 복원 */
	sigprocmask(SIG_UNBLOCK, &set, NULL);

	/* 4) 잘못된 인자 */
	CHECK(wait_signal(-1, 100) == -1);
	CHECK(wait_signal(SIGUSR2, -1) == -1);

	return drill_report();
}
