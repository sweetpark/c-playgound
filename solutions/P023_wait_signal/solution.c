#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <time.h>

/* P023 wait_signal — 모범답안 */
int wait_signal(int signo, int timeout_ms)
{
	sigset_t set, oldmask;
	struct timespec ts;
	int r, saved, result;

	if (signo <= 0 || timeout_ms < 0)
		return -1;
	sigemptyset(&set);
	if (sigaddset(&set, signo) != 0)
		return -1;

	/* 먼저 막고 나서 기다린다: 막지 않으면 "검사 후 대기" 사이에 도착한 시그널을 놓친다 */
	if (sigprocmask(SIG_BLOCK, &set, &oldmask) != 0)
		return -1;

	ts.tv_sec = timeout_ms / 1000;
	ts.tv_nsec = (long)(timeout_ms % 1000) * 1000000L;
	do {
		r = sigtimedwait(&set, NULL, &ts);
	} while (r < 0 && errno == EINTR);       /* 다른 시그널에 끊기면 (남은 시간은 단순화) 재시도 */

	if (r >= 0) {
		result = 1;
	} else if (errno == EAGAIN) {
		result = 0;                          /* 시간 초과 */
	} else {
		result = -1;
	}
	saved = errno;
	sigprocmask(SIG_SETMASK, &oldmask, NULL);   /* 원래 마스크로 복원 */
	errno = saved;
	return result;
}
