#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "drill_test.h"
#include "solution.c"

static void on_alarm(int s)
{
	static const char m[] = "FAIL (timeout/deadlock)\n";
	(void)s;
	if (write(1, m, sizeof(m) - 1) < 0) { /* ignore */ }
	_exit(1);
}

typedef struct {
	int fd;
	size_t total;
	uint32_t sum;
	int ordered;
} reader_t;

/* 일부러 느리게 읽어서 파이프가 가득 차게 만든다 → 비차단 write 는 부분 쓰기/EAGAIN */
static void *reader(void *arg)
{
	reader_t *r = arg;
	unsigned char buf[4096];
	ssize_t n;
	struct timespec ts = {0, 200000};   /* 0.2ms */

	r->ordered = 1;
	while ((n = read(r->fd, buf, sizeof(buf))) > 0) {
		ssize_t i;
		for (i = 0; i < n; i++) {
			if (buf[i] != (unsigned char)((r->total + (size_t)i) % 251)) r->ordered = 0;
			r->sum += buf[i];
		}
		r->total += (size_t)n;
		nanosleep(&ts, NULL);
	}
	return NULL;
}

int main(void)
{
	enum { N = 300000 };
	int fds[2];
	unsigned char *data;
	reader_t rd = {0, 0, 0, 0};
	pthread_t th;
	size_t i;
	uint32_t want = 0;
	int r;
	int fl;

	signal(SIGALRM, on_alarm);
	alarm(60);
	signal(SIGPIPE, SIG_IGN);

	data = malloc(N);
	CHECK(data != NULL);
	if (!data) return drill_report();
	for (i = 0; i < N; i++) {
		data[i] = (unsigned char)(i % 251);
		want += data[i];
	}

	CHECK(pipe(fds) == 0);
	fl = fcntl(fds[1], F_GETFL);
	fcntl(fds[1], F_SETFL, fl | O_NONBLOCK);       /* 쓰기 끝을 비차단으로 */
	rd.fd = fds[0];
	pthread_create(&th, NULL, reader, &rd);

	r = write_all(fds[1], data, N);
	CHECK(r == 0);
	close(fds[1]);
	pthread_join(th, NULL);
	close(fds[0]);

	CHECK(rd.total == N);                          /* 부분 쓰기·EAGAIN 에도 하나도 빠지지 않는다 */
	CHECK(rd.sum == want);
	CHECK(rd.ordered);

	/* 0 바이트 / 잘못된 인자 */
	CHECK(write_all(1, "", 0) == 0);
	CHECK(write_all(-1, "x", 1) == -1);
	CHECK(write_all(1, NULL, 5) == -1);

	/* 읽는 쪽이 닫히면 EPIPE → -1 (SIGPIPE 는 무시 중) */
	CHECK(pipe(fds) == 0);
	close(fds[0]);
	CHECK(write_all(fds[1], "abc", 3) == -1);
	close(fds[1]);

	free(data);
	return drill_report();
}
