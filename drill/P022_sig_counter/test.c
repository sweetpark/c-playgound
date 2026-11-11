#include <signal.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

int main(void)
{
	struct sigaction old;

	/* 설치 전에는 기본 동작 */
	CHECK(sigaction(SIGUSR1, NULL, &old) == 0);
	CHECK(old.sa_handler == SIG_DFL);

	CHECK(sigc_install(SIGUSR1) == 0);
	CHECK(sigc_count() == 0);

	/* sigaction 으로 설치했고(핸들러형, 1회용 아님, 처리 중 자기 자신 차단) */
	CHECK(sigaction(SIGUSR1, NULL, &old) == 0);
	CHECK(old.sa_handler != SIG_DFL && old.sa_handler != SIG_IGN);
	CHECK((old.sa_flags & SA_SIGINFO) == 0);
	CHECK((old.sa_flags & SA_RESETHAND) == 0);
	CHECK((old.sa_flags & SA_NODEFER) == 0);

	/* 설치되지 않았다면 이후 raise 가 프로세스를 죽이므로 여기서 중단 */
	if (old.sa_handler == SIG_DFL || old.sa_handler == SIG_IGN)
		return drill_report();

	/* raise 는 시그널이 처리된 뒤에 돌아온다 */
	raise(SIGUSR1);
	CHECK(sigc_count() == 1);
	raise(SIGUSR1);
	raise(SIGUSR1);
	CHECK(sigc_count() == 3);              /* 두 번째 이후에도 계속 센다 (signal() 의 1회용 동작이면 죽는다) */

	sigc_reset();
	CHECK(sigc_count() == 0);
	raise(SIGUSR1);
	CHECK(sigc_count() == 1);

	/* 해제하면 기본 동작으로 복원 */
	CHECK(sigc_uninstall(SIGUSR1) == 0);
	CHECK(sigaction(SIGUSR1, NULL, &old) == 0);
	CHECK(old.sa_handler == SIG_DFL);

	/* 잡을 수 없는 시그널 / 잘못된 번호 */
	CHECK(sigc_install(SIGKILL) == -1);
	CHECK(sigc_install(-5) == -1);
	CHECK(sigc_install(100000) == -1);

	return drill_report();
}
