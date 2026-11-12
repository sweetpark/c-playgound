#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "drill_test.h"
#include "shmprobe.h"
#include "solution.c"

int main(void)
{
	int id, st, pfd[2];
	char *a, *b;
	pid_t pid;

	/* [1] 만들기: 아직 아무도 안 붙었다 */
	id = shm_make(4096);
	CHECK(id >= 0);
	if (id < 0)
		return drill_report();
	CHECK(shm_exists(id) == 1);
	CHECK(shm_nattch(id) == 0);
	CHECK(shm_marked_dead(id) == 0);

	/* [2] 붙이면 nattch 증가, 같은 세그먼트를 두 번 붙이면 같은 내용이 보인다 (주소는 다를 수 있다) */
	a = shm_map(id);
	b = shm_map(id);
	CHECK(a != NULL && b != NULL);
	CHECK(shm_nattch(id) == 2);
	if (a && b) {
		strcpy(a, "hello");
		CHECK(strcmp(b, "hello") == 0);
	}
	CHECK(shm_unmap(b) == 0);
	CHECK(shm_nattch(id) == 1);

	/* [3] 자식이 붙은 채 kill -9 로 죽으면 커널이 알아서 떼어 준다 → nattch 가 원래대로 */
	CHECK(pipe(pfd) == 0);
	pid = fork();
	if (pid == 0) {
		char *c = shm_map(id);
		char ok = (c != NULL);
		(void)write(pfd[1], &ok, 1);
		pause();
		_exit(0);
	}
	{
		char ok = 0;
		CHECK(read(pfd[0], &ok, 1) == 1 && ok == 1);
	}
	/* fork 하면 부모가 붙여 둔 매핑(a)이 자식에게도 상속된다: 부모 1 + 상속 1 + 자식이 새로 붙인 1 = 3 */
	CHECK(shm_nattch(id) == 3);
	kill(pid, SIGKILL);
	waitpid(pid, &st, 0);
	CHECK(shm_nattch(id) == 1);
	CHECK(shm_exists(id) == 1); /* 세그먼트 자체는 프로세스가 죽어도 남는다 */
	close(pfd[0]);
	close(pfd[1]);

	/* [4] 삭제를 예약했는데 아직 붙어 있다 → 바로 사라지지 않고 '삭제 예약' 상태. 붙어 있는 쪽은 계속 쓸 수 있다 */
	CHECK(shm_remove(id) == 0);
	CHECK(shm_exists(id) == 1);
	CHECK(shm_marked_dead(id) == 1);
	if (a) {
		strcpy(a, "still ok");
		CHECK(strcmp(a, "still ok") == 0);
	}

	/* [5] 마지막으로 떼는 순간 진짜 사라진다 */
	CHECK(shm_unmap(a) == 0);
	CHECK(shm_exists(id) == 0);
	CHECK(shm_nattch(id) < 0);
	CHECK(shm_remove(id) < 0);

	/* [6] 안 붙은 세그먼트는 IPC_RMID 즉시 사라진다 */
	id = shm_make(8192);
	CHECK(id >= 0);
	CHECK(shm_remove(id) == 0);
	CHECK(shm_exists(id) == 0);

	return drill_report();
}
