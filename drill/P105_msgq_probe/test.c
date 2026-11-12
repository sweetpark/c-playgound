#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#include "drill_test.h"
#include "mqprobe.h"
#include "solution.c"

int main(void)
{
	int q, sent = 0, rc, st;
	unsigned long n = 99, b = 99, cap = 99;

	/* [1] 만들기: 용량을 4096 으로 줄인다 */
	q = mq_create(4096);
	CHECK(q >= 0);
	if (q < 0)
		return drill_report();
	CHECK(mq_stat(q, &n, &b, &cap) == 0);
	CHECK(n == 0 && b == 0 && cap == 4096);

	/* [2] 하나 보내면 개수 1, 바이트 = 메시지 길이(헤더 mtype 은 세지 않는다) */
	CHECK(mq_send_one(q, 100) == 0);
	CHECK(mq_stat(q, &n, &b, NULL) == 0);
	CHECK(n == 1 && b == 100);

	/* [3] 가득 찰 때까지: 4096/100 = 40개까지 들어가고, 그다음은 EAGAIN (기다리지 않는다) */
	rc = mq_fill(q, 100, &sent);
	CHECK(rc == -EAGAIN);
	CHECK(sent == 39); /* 이미 1개 있었으니 39개 더 */
	CHECK(mq_stat(q, &n, &b, NULL) == 0);
	CHECK(n == 40 && b == 4000);
	CHECK(mq_send_one(q, 100) == -EAGAIN);
	CHECK(mq_send_one(q, 96) == 0); /* 남은 96바이트에는 96바이트짜리가 정확히 들어간다 */
	CHECK(mq_send_one(q, 1) == -EAGAIN);

	/* [4] 한 개 크기가 시스템 한도(msgmax)를 넘으면 큐 상태와 무관하게 EINVAL */
	{
		unsigned long b2;
		int q2 = mq_create(1 << 20);
		CHECK(q2 >= 0);
		CHECK(mq_send_one(q2, 70000) < 0); /* 8192 초과(우리 버퍼 한계) → -EINVAL */
		CHECK(mq_stat(q2, NULL, &b2, NULL) == 0);
		CHECK(b2 == 0);
		CHECK(mq_remove(q2) == 0);
	}

	/* [5] 보낸 프로세스가 죽어도 큐와 메시지는 커널에 남는다 */
	{
		int q4 = mq_create(4096);
		pid_t pid;
		CHECK(q4 >= 0);
		pid = fork();
		if (pid == 0) {
			mq_send_one(q4, 50);
			mq_send_one(q4, 50);
			kill(getpid(), SIGKILL); /* 정상 종료 없이 죽는다 */
		}
		waitpid(pid, &st, 0);
		CHECK(WIFSIGNALED(st));
		CHECK(mq_stat(q4, &n, &b, NULL) == 0);
		CHECK(n == 2 && b == 100);
		CHECK(mq_drain(q4) == 2);
		CHECK(mq_stat(q4, &n, &b, NULL) == 0);
		CHECK(n == 0 && b == 0);
		CHECK(mq_remove(q4) == 0);
		CHECK(mq_stat(q4, &n, &b, NULL) < 0); /* 지운 뒤에는 조회도 실패 */
	}

	/* [6] drain 은 개수를 돌려주고, 빈 큐에서는 기다리지 않고 0 */
	CHECK(mq_drain(q) == 41);
	CHECK(mq_drain(q) == 0);
	CHECK(mq_send_one(q, 100) == 0); /* 비웠으니 다시 들어간다 */
	CHECK(mq_remove(q) == 0);
	CHECK(mq_remove(q) < 0); /* 두 번 지우면 실패 */

	return drill_report();
}
